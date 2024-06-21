#include "../pch.h"
#include "InputManager.h"
#include "Registry.h"
#include "Memory.h"

//TODO: Restart winlogon.exe when it doesn't exist.

/**
 * 初始化键盘钩子相关设置。
 * 本函数通过查询操作系统版本和特定进程的内存信息，来确定键盘状态变量的地址。
 * 对于不同的Windows版本，采用不同的方法来获取这个地址。
 *
 * @return 返回true表示初始化成功，返回false表示初始化失败。
 */
bool c_keys::InitKeyboard()
{
    // 从注册表查询当前Windows版本号
	std::string win = registry.QueryValue("HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\CurrentBuild", e_registry_type::sz);
	int Winver = 0;
	if (!win.empty())
		Winver = std::stoi(win);
	else
		return false;

    // 获取winlogon进程的PID，用于后续的内存操作
	this->win_logon_pid = mem.GetPidFromName("winlogon.exe");

    // 对于Windows版本高于22000的情况，尝试从csrss.exe进程中获取键盘状态变量的地址
	if (Winver > 22000)
	{
		auto pids = mem.GetPidListFromName("csrss.exe");
		for (size_t i = 0; i < pids.size(); i++)
		{
			auto pid = pids[i];
			uintptr_t tmp = VMMDLL_ProcessGetModuleBaseU(mem.vHandle, pid, const_cast<LPSTR>("win32ksgd.sys"));
			uintptr_t g_session_global_slots = tmp + 0x3110;
			uintptr_t user_session_state = mem.Read<uintptr_t>(mem.Read<uintptr_t>(mem.Read<uintptr_t>(g_session_global_slots, pid), pid), pid);
			gafAsyncKeyStateExport = user_session_state + 0x3690;
			if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
				break;
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
			return true;
		return false;
	}
	else
	{
        // 对于Windows版本低于或等于22000的情况，采用另一种方法来获取键盘状态变量的地址
		PVMMDLL_MAP_EAT eat_map = NULL;
		PVMMDLL_MAP_EATENTRY eat_map_entry;
		bool result = VMMDLL_Map_GetEATU(mem.vHandle, mem.GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, const_cast<LPSTR>("win32kbase.sys"), &eat_map);
		if (!result)
			return false;

		if (eat_map->dwVersion != VMMDLL_MAP_EAT_VERSION)
		{
			VMMDLL_MemFree(eat_map);
			eat_map_entry = NULL;
			return false;
		}

		for (int i = 0; i < eat_map->cMap; i++)
		{
			eat_map_entry = eat_map->pMap + i;
			if (strcmp(eat_map_entry->uszFunction, "gafAsyncKeyState") == 0)
			{
				gafAsyncKeyStateExport = eat_map_entry->vaFunction;

				break;
			}
		}

		VMMDLL_MemFree(eat_map);
		eat_map = NULL;
		if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		{
			PVMMDLL_MAP_MODULEENTRY module_info;
			auto result = VMMDLL_Map_GetModuleFromNameW(mem.vHandle, mem.GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, static_cast<LPCWSTR>(L"win32kbase.sys"), &module_info, VMMDLL_MODULE_FLAG_NORMAL);
			if (!result)
			{
				LOG("failed to get module info\n");
				return false;
			}

			char str[32];
			if (!VMMDLL_PdbLoad(mem.vHandle, mem.GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, module_info->vaBase, str))
			{
				LOG("failed to load pdb\n");
				return false;
			}

			uintptr_t gafAsyncKeyState;
			if (!VMMDLL_PdbSymbolAddress(mem.vHandle, str, const_cast<LPSTR>("gafAsyncKeyState"), &gafAsyncKeyState))
			{
				LOG("failed to find gafAsyncKeyState\n");
				return false;
			}
			LOG("found gafAsyncKeyState at: 0x%p\n", gafAsyncKeyState);
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
			return true;
		return false;
	}
}



/**
 * 更新按键状态。
 * 该函数通过与前一按键状态进行比较，来更新当前的按键状态。它首先复制当前状态到一个临时数组，
 * 然后从内存中读取最新的按键状态。接着，它遍历所有虚拟键码，比较当前状态和前一状态，
 * 如果发现某个键被按下（从释放状态变为按压状态），则在之前的按键状态数组中标记这个键为按压状态。
 * 这样做的目的是为了后续能够准确地识别按键的按下和释放事件。
 */
void c_keys::UpdateKeys()
{
    // 初始化前一按键状态数组，所有键均处于释放状态
    uint8_t previous_key_state_bitmap[64] = {0};
    // 复制当前按键状态到前一按键状态数组
    memcpy(previous_key_state_bitmap, state_bitmap, 64);

    // 从指定内存位置读取最新的按键状态到当前状态数组
    VMMDLL_MemReadEx(mem.vHandle, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gafAsyncKeyStateExport, reinterpret_cast<PBYTE>(&state_bitmap), 64, NULL, VMMDLL_FLAG_NOCACHE);

    // 遍历所有虚拟键码，检查按键状态的变化
    for (int vk = 0; vk < 256; ++vk)
        // 如果当前键被按下且前一状态为释放，则在前一状态数组中标记这个键为按压状态
        if ((state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2) && !(previous_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2))
            previous_state_bitmap[vk / 8] |= 1 << vk % 8;
}


/**
 * 检查指定的虚拟键是否处于按下状态。
 *
 * @param virtual_key_code 虚拟键码，用于标识要检查的键。
 * @return 如果键处于按下状态，则返回true；否则返回false。
 */
bool c_keys::IsKeyDown(uint32_t virtual_key_code)
{
    // 检查gafAsyncKeyStateExport的值是否有效，如果无效，则认为键状态不可用，直接返回false。
    if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
        return false;

    // 检查是否需要更新键状态。如果当前时间与上一次更新时间间隔超过1毫秒，则更新键状态并更新起始时间。
    if (std::chrono::system_clock::now() - start > std::chrono::milliseconds(1))
    {
        UpdateKeys(); // 更新键状态。
        start = std::chrono::system_clock::now(); // 更新起始时间为当前时间。
    }

    // 通过位操作检查指定的虚拟键是否按下。
    // state_bitmap是一个位图，用于存储键的状态。这里通过计算索引来访问对应的位。
    return state_bitmap[(virtual_key_code * 2 / 8)] & 1 << virtual_key_code % 4 * 2;
}

