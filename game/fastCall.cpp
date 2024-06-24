

#include "address.h"
#include "Memory.h"
#include "fastCall.h"
INT32 g_Hook接口;
INT32 g_Call最大长度;
INT32 g_RSP;
INT32 g_超时调用设置;
INT64 g_分配空间;
INT64 g_上次空间;
INT64 g_挂钩地址;
INT64 g_旧数据;
INT64 g_旧数据保存;
INT64 g_中转框架内存;
INT64 g_执行函数代码段;
INT64 g_执行函数result;
INT64 g_执行函数控制符;
INT64 g_执行函数刷新Time;
INT64 g_执行函数上次Time;
INT64 g_执行函数数据段;
INT64 g_hook框架段;
CRITICAL_SECTION 全局_lock_tea;
INT64 分配空间(INT32 分配长度)
{
	INT64 result;
	result = g_上次空间;
	g_上次空间 = g_上次空间 + 分配长度;
	return result;
}

void InitHook_Type1(int 接口选择)
{
	g_旧数据保存 = 分配空间(8);
	INT64 挂钩地址;
	if (接口选择 == 1)
	{
		挂钩地址 = mem.readLong(GameTranslateMessage);
		// 09 E1 17 06 00 00 00 00 F9
		挂钩地址 = 挂钩地址 + mem.readInt(挂钩地址 + 2) + 6;
		g_挂钩地址 = 挂钩地址;
	}
	g_旧数据 = mem.readLong(g_旧数据保存) == 0 ? mem.readLong(g_挂钩地址) : mem.readLong(g_旧数据保存);
}

void 调用等待()
{
	while (mem.readInt(g_执行函数控制符) == 1)
	{
		Sleep(1);
	}
	while (mem.readInt(g_执行函数控制符) == 2)
	{
		if (g_超时调用设置 != 0 && mem.readInt(g_执行函数刷新Time) - mem.readInt(g_执行函数上次Time) > g_超时调用设置)
		{
			break;
		}
		Sleep(1);
	}
}

// 调用shellcode
INT64 调用函数_正常(vector<BYTE> 调用数据)
{
	INT64 result;
	调用数据 = AddBytes(调用数据, vector<BYTE>{ 195 });
	if (调用数据.size() > g_Call最大长度)
	{

		//MessageBoxExW(0, L"调用数过长", 0, MB_DEFAULT_DESKTOP_ONLY, 0);
		return 0;
	}

	//EnterCriticalSection(&全局_lock_tea);
	调用等待();
	mem.WriteBytes(g_执行函数数据段, 调用数据);
	mem.Write(g_执行函数控制符, 1);
	调用等待();
	mem.WriteBytes(g_执行函数数据段, 取空白ByteArr(调用数据.size()));
	result = mem.readLong(g_执行函数result);
	//LeaveCriticalSection(&全局_lock_tea);
	return result;
}

INT64 调用函数_自动找堆栈(vector<BYTE> 调用数据, int rsp)
{
	if (调用数据[调用数据.size() - 1] == 195)
	{
		调用数据[调用数据.size() - 1] = 144;
	}

	vector<BYTE> 汇编数据 = AddBytes(vector<BYTE>{ 72, 129, 236 }, _IntToBytes(rsp,4), 调用数据);
	汇编数据 = AddBytes(汇编数据,vector<BYTE>{ 72, 129, 196 }, _IntToBytes(rsp,4));
	return 调用函数_正常(汇编数据);
}

void InitCode()
{
	vector<BYTE> code;

	//InitializeCriticalSection(&全局_lock_tea);

	g_Hook接口 = 1;
	g_Call最大长度 = 6666;
	g_RSP = 584;
	auto 模块列表 = mem.GetModuleList("DNF.exe");
	g_分配空间 = 全局空白; //VirtualAllocAddress(4096 * 1024);
	g_超时调用设置 = 1000 * 60;
	g_上次空间 = g_分配空间;

	code = vector<BYTE>{ 72, 137, 116, 36, 8, 72, 137, 124, 36, 16, 65, 86, 72, 131, 236, 32, 72, 190, 0, 0, 0, 64, 1, 0, 0, 0, 72, 191, 118, 11, 204, 63, 1, 0, 0, 0, 73, 190, 126, 11, 204, 63, 1, 0, 0, 0, 255, 214, 72, 163, 142, 11, 204, 63, 1, 0, 0, 0, 131, 63, 1, 117, 57, 73, 199, 6, 0, 0, 0, 0, 72, 199, 7, 2, 0, 0, 0, 255, 214, 72, 163, 134, 11, 204, 63, 1, 0, 0, 0, 65, 86, 87, 83, 86, 72, 184, 0, 0, 0, 80, 1, 0, 0, 0, 255, 208, 94, 91, 95, 65, 94, 73, 137, 6, 199, 7, 0, 0, 0, 0, 72, 139, 116, 36, 48, 72, 139, 124, 36, 56, 72, 131, 196, 32, 65, 94, 195 };

	g_执行函数代码段 = 分配空间(code.size());
	g_执行函数数据段 = 分配空间(g_Call最大长度);
	g_执行函数控制符 = 分配空间(8);
	g_执行函数刷新Time = 分配空间(8);
	g_执行函数result = 分配空间(8);
	g_执行函数上次Time = 分配空间(8);
	mem.WriteBytes(g_执行函数代码段, code);
	mem.Write(g_执行函数代码段 + 0x10 + 2, mem.readLong(GameTimeGetTime));
	mem.Write(g_执行函数代码段 + 0x1A + 2, g_执行函数控制符);
	mem.Write(g_执行函数代码段 + 0x24 + 2, g_执行函数result);
	mem.Write(g_执行函数代码段 + 0x30 + 2, g_执行函数刷新Time);
	mem.Write(g_执行函数代码段 + 0x4F + 2, g_执行函数上次Time);
	mem.Write(g_执行函数代码段 + 0x5E + 2, g_执行函数数据段);

	code = vector<BYTE>{ 72, 137, 92, 36, 8, 72, 137, 116, 36, 16, 87, 72, 131, 236, 32 };
	code = AddBytes(code, vector<BYTE>{ 72, 184 }, _IntToBytes(g_执行函数代码段,8));
	code = AddBytes(code, vector<BYTE>{ 255, 208 }, vector<BYTE>{ 72, 139, 92, 36, 48, 72, 139, 116, 36, 56, 72, 131, 196, 32, 95, 195 });

	g_中转框架内存 = 分配空间(code.size());
	mem.WriteBytes(g_中转框架内存, code);
	InitHook_Type1(g_Hook接口);

	code = vector<BYTE>{ 80, 83, 81, 82, 87, 86, 85, 65, 80, 65, 81, 65, 82, 65, 83, 65, 84, 65, 85, 65, 86, 65, 87, 156, 72, 131, 236, 40 };
	code = AddBytes(code, vector<BYTE>{ 72, 184 }, _IntToBytes(g_中转框架内存,8));
	code = AddBytes(code, vector<BYTE>{ 255, 208 }, vector<BYTE>{ 72, 131, 196, 40, 157, 65, 95, 65, 94, 65, 93, 65, 92, 65, 91, 65, 90, 65, 89, 65, 88, 93, 94, 95, 90, 89, 91, 88 });
	code = AddBytes(code, vector<BYTE>{ 255, 37, 0, 0, 0, 0 }, _IntToBytes(g_旧数据,8));

	g_hook框架段 = 分配空间(code.size());
	mem.WriteBytes(g_hook框架段, code);
	mem.Write(g_旧数据保存, g_旧数据);
	mem.Write(g_挂钩地址, g_hook框架段);
}

// 结束后调用
void FreeCode()
{
	mem.Write(g_挂钩地址, g_旧数据);
	mem.WriteBytes(g_中转框架内存, 取空白ByteArr(g_上次空间 - g_分配空间));
	DeleteCriticalSection(&全局_lock_tea);
}

void 强制释放许可证()
{
	LeaveCriticalSection(&全局_lock_tea);
}



