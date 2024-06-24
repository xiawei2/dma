#pragma once
#include<cstdio>
#include <basetsd.h>
#include <vector>
#include "utils.h"
typedef unsigned char BYTE;
extern INT32 g_Hook接口;
extern INT32 g_Call最大长度;
extern INT32 g_RSP;
extern INT32 g_超时调用设置;
extern INT64 g_分配空间;
extern INT64 g_上次空间;
extern INT64 g_挂钩地址;
extern INT64 g_旧数据;
extern INT64 g_旧数据保存;
extern INT64 g_中转框架内存;
extern INT64 g_执行函数代码段;
extern INT64 g_执行函数result;
extern INT64 g_执行函数控制符;
extern INT64 g_执行函数刷新Time;
extern INT64 g_执行函数上次Time;
extern INT64 g_执行函数数据段;
extern INT64 g_hook框架段;

INT64 分配空间(INT32 分配长度);
void InitHook_Type1(int 接口选择);
void 调用等待();

INT64 调用函数_正常(vector<BYTE> 调用数据);
INT64 调用函数_自动找堆栈(vector<BYTE> 调用数据, int rsp = g_RSP);
void InitCode();
void FreeCode();
void 强制释放许可证();

template <typename... Args>
INT64 FastCall(INT64 func, Args... args) {
    vector<INT64> 参数数组 = { args... };

    short 指令集[4] = { static_cast<short>(47432), static_cast<short>(47688), static_cast<short>(47177), static_cast<short>(47433) };
    vector<BYTE> code;

    for (int i = 0; i < 参数数组.size(); i++)
    {
        int index = 参数数组.size() - i - 1;
        if (index <= 3)
        {
            code = AddBytes(code, _IntToBytes(指令集[index]));
            code = AddBytes(code, _IntToBytes(参数数组[index]));
        }
        else
        {
            code = AddBytes(code, vector<BYTE>{ 72, 184 });
            code = AddBytes(code, _IntToBytes(参数数组[index]));
            code = AddBytes(code, vector<BYTE>{ 72, 137, 132, 36 });
            code = AddBytes(code, _IntToBytes(i * 8));
        }
    }

    vector<BYTE> tempCode = AddBytes(code, vector<BYTE>{ 72, 184 }, _IntToBytes(func));
    tempCode = AddBytes(tempCode, vector<BYTE>{ 255, 208 });
    int rsp = 参数数组.size() < 4 ? 4 * 8 + 8 : 参数数组.size() * 8 + 8;
    rsp = rsp / 8 % 2 == 0 ? rsp + 8 : rsp;
    code = AddBytes(vector<BYTE>{ 72, 129, 236 }, _IntToBytes(rsp), tempCode);
    code = AddBytes(code,vector<BYTE>{ 72, 129, 196 }, _IntToBytes(rsp));
    return 调用函数_正常(code);
}
