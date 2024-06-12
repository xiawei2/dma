#include <algorithm>
#include "../pch.h"
#include "Shellcode.h"
#include "Memory.h"

std::vector<std::string> blacklist = {"kernel32.dll", "kernelbase.dll", "wow64.dll", "wow64win.dll", "wow64cpu.dll", "ntoskrnl.exe", "win32kbase.sys"};

/**
 * 寻找合适的codecave来注入shellcode。
 *
 * @param function_size 函数的大小，用于确定codecave是否足够大。
 * @param process_name 目标进程的名称。
 * @param module 模块的名称，在其中寻找codecave。
 * @return 返回找到的codecave的地址，如果未找到则返回0。
 */
uint64_t c_shellcode::find_codecave(size_t function_size, const std::string& process_name, const std::string& module)
{
    /* 获取目标进程的PID */
    int pid = mem.GetPidFromName(process_name);

    /* 初始化VMMDLL_PROCESS_INFORMATION结构体 */
    VMMDLL_PROCESS_INFORMATION process_info = {0};
    process_info.magic = VMMDLL_PROCESS_INFORMATION_MAGIC;
    process_info.wVersion = VMMDLL_PROCESS_INFORMATION_VERSION;
    SIZE_T process_info_size = sizeof(VMMDLL_PROCESS_INFORMATION);

    /* 获取进程信息 */
    if (!VMMDLL_ProcessGetInformation(mem.vHandle, pid, &process_info, &process_info_size))
    {
        /* 如果获取信息失败，记录日志并返回0 */
        LOG("[!] Could not retrieve process for PID: %i", pid);
        return 0;
    }

    /* 初始化获取的段数量变量 */
    DWORD cSections = 0;

    /* 获取模块的段信息 */
    if (!VMMDLL_ProcessGetSectionsU(mem.vHandle, pid, const_cast<LPSTR>(module.c_str()), NULL, 0, &cSections) || !cSections)
    {
        /* 如果获取段信息失败，记录日志并返回0 */
        LOG("[!] Could not retrieve sections #1 for '%s'\n", module.c_str());
        return 0;
    }

    /* 分配内存来存储段信息 */
    const PIMAGE_SECTION_HEADER pSections = static_cast<PIMAGE_SECTION_HEADER>(LocalAlloc(LMEM_ZEROINIT, cSections * sizeof(IMAGE_SECTION_HEADER)));

    /* 再次获取段信息 */
    if (!pSections || !VMMDLL_ProcessGetSectionsU(mem.vHandle, pid, const_cast<LPSTR>(module.c_str()), pSections, cSections, &cSections) || !cSections)
    {
        /* 如果获取段信息失败，记录日志并返回0 */
        LOG("[!] Could not retrieve sections #2 for '%s'\n", module.c_str());
        return 0;
    }

    /* 遍历段信息寻找合适的codecave */
    /*Scan for code cave*/
    uint64_t codecave = 0;
    for (int i = 0; i < cSections; i++)
    {
        /* 检查段是否可执行、可写入、可读取，并且大小足够 */
        if (!codecave && ((pSections[i].Characteristics & (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_READ))) && ((pSections[i].Misc.VirtualSize & 0xfff) < (0x1000 - function_size)))
        {
            /* 计算codecave的地址 */
            codecave = VMMDLL_ProcessGetModuleBaseU(mem.vHandle, pid, const_cast<LPSTR>(module.c_str())) + ((pSections[i].VirtualAddress + pSections[i].Misc.VirtualSize)) + 0x10;
            if (!codecave)
                break;
        }
    }

    /* 如果没有找到合适的codecave，记录日志并返回0 */
    if (!codecave)
    {
        LOG("[!] Could not find a code cave for '%s'\n", module.c_str());
        return 0;
    }

    /* 分配内存来存储从codecave读取的数据 */
    auto buffer = std::unique_ptr<uint8_t[]>(new uint8_t[function_size]);

    /* 从codecave读取数据 */
    if (!mem.Read(codecave, buffer.get(), function_size, pid))
    {
        /* 如果读取失败，记录日志并返回0 */
        LOG("[!] Could not read codecave for '%s'\n", module.c_str());
        return 0;
    }

    /* 检查codecave是否全部由0组成，确保其大小足够 */
    for (size_t i = 0; i < function_size; i++)
    {
        if (buffer[i] != 0x0)
        {
            /* 如果codecave不满足条件，记录日志并返回0 */
            LOG("[!] Codecave isn't big enough for the shellcode.\n");
            return 0;
        }
    }
    LOG("[!] Codecave fond at %s\n",module.c_str());
    /* 返回找到的codecave的地址 */
    return codecave;
}


/**
 * 在指定进程中查找所有适合存放shellcode的代码洞。
 *
 * @param function_size 需要存放的shellcode的大小。
 * @param process_name 目标进程的名称。
 * @return 返回一个包含所有适合存放shellcode的代码洞地址的向量。
 */
std::vector<uint64_t> c_shellcode::find_all_codecave(size_t function_size, const std::string& process_name)
{
    // 初始化一个空的代码洞地址向量
	std::vector<uint64_t> codecaves = { };

    // 获取目标进程的所有模块列表
	std::vector<std::string> module_list = mem.GetModuleList(process_name);

    // 遍历模块列表
	for (size_t i = 0; i < module_list.size(); i++)
	{
        // 检查当前模块是否在黑名单中，如果在则跳过
		if (std::find(blacklist.begin(), blacklist.end(), module_list[i]) != blacklist.end())
			continue;

        // 获取当前模块的名称
		std::string module = module_list[i];

        // 在当前模块中查找适合存放shellcode的代码洞
		uint64_t codecave = find_codecave(function_size, process_name, module);

        // 如果找到了合适的代码洞，则添加到结果向量中
		if (!codecave)
			continue;
		codecaves.push_back(codecave);
	}

    // 返回所有找到的代码洞地址向量
	return codecaves;
}


/**
 * 在指定进程中通过挂钩方式调用指定函数。
 *
 * @param hook 挂钩函数所在的地址。
 * @param function 要挂钩的目标函数地址。
 * @param process_name 目标进程的名称。
 * @return 如果函数调用成功则返回true，否则返回false。
 */
bool c_shellcode::call_function(void* hook, void* function, const std::string& process_name)
{
    // 根据进程名获取进程ID
    int pid = mem.GetPidFromName(process_name);

    // 定义一个字节数组存储x86-64跳转指令，用于跳转到挂钩函数
    BYTE jmp_bytes[14] = {
        0xff, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp [RIP+0x00000000]
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // RIP值预留位置
    };

    // 设置跳转指令的目的地址为挂钩函数地址
    *reinterpret_cast<uint64_t*>(jmp_bytes + 6) = reinterpret_cast<uintptr_t>(hook);

    // 创建一个唯一指针以存储目标函数地址处的原始字节
    auto orig_bytes = std::unique_ptr<uint8_t[]>(new uint8_t[sizeof(jmp_bytes)]);

    // 读取目标函数地址处的原始字节
    if (!mem.Read(reinterpret_cast<uintptr_t>(function), orig_bytes.get(), sizeof(jmp_bytes), pid))
        return false;

    // 将跳转指令的字节写入目标函数地址，实现挂钩
    if (!VMMDLL_MemWrite(mem.vHandle, pid, reinterpret_cast<uintptr_t>(function), jmp_bytes, sizeof(jmp_bytes)))
    {
        LOG("[!] 1 写入内存失败于地址 0x%p\n", function);
        return false;
    }

    // 暂停100毫秒，允许挂钩生效
    Sleep(100);

    // 还原目标函数的原始字节，取消挂钩
    if (!VMMDLL_MemWrite(mem.vHandle, pid, reinterpret_cast<uintptr_t>(function), orig_bytes.get(), sizeof(jmp_bytes)))
    {
        LOG("[!] 2 写入内存失败于地址 0x%p\n", function);
        return false;
    }

    return true;
}

