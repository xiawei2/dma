//
// Created by 17129 on 2024/6/14.
//

#ifndef DMALIB_GAMECALL_H
#define DMALIB_GAMECALL_H


#include "allHeader.h"

class GameCall {
public:
    ByteArr GetNewByteAry(int num);

    int64_t GetPerPtrCall(int64_t address);

    std::vector<BYTE> SubRsp(int i);

    std::vector<BYTE> AddRsp(int i);

    vector<BYTE> Call(int64_t address);

    void CompileCall(vector<BYTE> 汇编代码);

    int64_t 取人物指针Call(long long int 空白地址);
};


#endif //DMALIB_GAMECALL_H
