//
// Created by 17129 on 2024/6/14.
//

#include "GameCall.h"
#include "allHeader.h"
#include "utils.h"
#include "address.h"
VOID GameCall::CompileCall(ByteArr 汇编代码)
{
    __int64 static 局_空白地址;
    if (局_空白地址 == 0)
    {
        局_空白地址 = (__int64)mem.FindCodecave(2048);
    }
    __int64 汇编中转 = 局_空白地址 + 300;
    __int64 空白地址 = 局_空白地址 + 500;
    __int64 判断地址 = 空白地址 - 100;
    static bool 异步执行;
    if (异步执行) {
        return;
    }
    异步执行 = true;

    __int64 Hook汇编 = 内存汇编;
    Hook汇编 = Hook汇编 + 144;
    __int64 Hook跳回 = Hook汇编 + 19;
    ByteArr Hook数据 = mem.readBytes(Hook汇编, 19);
    ByteArr Hook原数据 = Hook数据;

    Hook数据 = AddBytes(Hook数据, ByteArr{ 72, 184 });
    Hook数据 = AddBytes(Hook数据, _IntToBytes(判断地址, 8));

    Hook数据 = AddBytes(Hook数据, ByteArr{ 131, 56, 1, 117, 42, 72, 129, 236, 0, 3, 0, 0 });

    Hook数据 = AddBytes(Hook数据, ByteArr{ 72, 187 });
    Hook数据 = AddBytes(Hook数据, _IntToBytes(空白地址, 8));

    Hook数据 = AddBytes(Hook数据, ByteArr{ 255, 211 });

    Hook数据 = AddBytes(Hook数据, ByteArr{ 72, 184 });
    Hook数据 = AddBytes(Hook数据, _IntToBytes(判断地址, 8));

    Hook数据 = AddBytes(Hook数据, ByteArr{ 199, 0, 3, 0, 0, 0 });
    Hook数据 = AddBytes(Hook数据, ByteArr{ 72, 129, 196, 0, 3, 0, 0 });

    Hook数据 = AddBytes(Hook数据, ByteArr{ 255, 37, 0, 0, 0, 0 });
    Hook数据 = AddBytes(Hook数据, _IntToBytes(Hook跳回, 8));

    if (mem.readInt(汇编中转) == 0) {
        mem.WriteBytes(汇编中转, Hook数据);
    }
    Hook数据.clear();

    mem.WriteBytes(空白地址, AddBytes(汇编代码, ByteArr{ 195 }));
    ByteArr tmp = { 255, 37, 0, 0, 0, 0 };
    tmp = AddBytes(tmp, _IntToBytes(汇编中转, 8));
    tmp = AddBytes(tmp, ByteArr{ 144, 144, 144, 144, 144 });
    mem.WriteBytes(Hook汇编, tmp);
    mem.Write(判断地址, 1);
    while (mem.readLong(判断地址) == 1)
    {
        Sleep(10);
    }
    mem.WriteBytes(Hook汇编, Hook原数据);
    mem.WriteBytes(空白地址, 取空白ByteArr(sizeof(汇编代码) + 16));

    异步执行 = false;
}
int64_t GameCall::取人物指针Call(__int64 空白地址)
{
    ByteArr shellCode = { 72, 131, 236, 100 };  // sub rsp,100

    shellCode = AddBytes(shellCode, ByteArr{72, 184});  // mov rax  人物call
    shellCode = AddBytes(shellCode, _IntToBytes(人物CALL, 8));

    shellCode = AddBytes(shellCode, ByteArr{255, 208});  // CALL rax

    shellCode = AddBytes(shellCode, ByteArr{72, 163});
    shellCode = AddBytes(shellCode, _IntToBytes(空白地址, 8));

    shellCode = AddBytes(shellCode, ByteArr{72, 131, 196, 100});  // add rsp,100
    CompileCall(shellCode);
    __int64 返回地址 = mem.readLong(空白地址);
    return 返回地址;
}


ByteArr GameCall::GetNewByteAry(int num) {
    ByteArr res;
    for (size_t i = 0; i < num; i++)
    {
        res.push_back(0);
    }
    return res;
}

/**
 * 取人物指针call
 *
 * @param address long
 * @return long
 */
int64_t GameCall::GetPerPtrCall(int64_t address) {
    // 构造 shellCode 的字节数组
    auto shellCode = AddBytes(AddBytes(SubRsp(100), Call(人物CALL)), {72, 163});
    shellCode = AddBytes(shellCode,IntToBytes(address));
    shellCode = AddBytes(shellCode, AddRsp(100));
    CompileCall(shellCode);
    return mem.readLong(address);
}


ByteArr GameCall::SubRsp(int i) {
    if (i > 127) {
        return AddBytes(ByteArr{72, 129, 236}, _IntToBytes(i,4));
    }
    return AddBytes(ByteArr{72, 131, 236}, {static_cast<unsigned char>(i)});
}

ByteArr GameCall::AddRsp(int i) {
    if (i > 127) {
        return AddBytes(ByteArr{72, 129, 196}, _IntToBytes(i,4));
    }
    return AddBytes(ByteArr{72, 131, 196}, {static_cast<unsigned char>(i)});
}

ByteArr GameCall::Call(int64_t address) {
    auto shellCode = ByteArr{255, 21, 2, 0, 0, 0, 235, 8};
    return AddBytes(shellCode, IntToBytes(address));
}