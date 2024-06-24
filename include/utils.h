//
// Created by 17129 on 2024/6/14.
//
#pragma once
#ifndef DMA_UTILS_H
#define DMA_UTILS_H
#include <windows.h>
#include <random>
#include <locale>
#include <codecvt>
#include <stringapiset.h>
//#include "allHeader.h"
//#include <minwindef.h>
#include "Coordinate.h"
#include "allHeader.h"

class utils {

};

#endif //DMA_UTILS_H

std::wstring UnicodeToAnsi(std::vector<BYTE> bytes);
bool ArePointsEqual(Coordinate index1, Coordinate index2);

int rnd(int min, int max);
void split(const std::wstring& str, std::vector<std::wstring>& tokens, const std::wstring& delimiters);
std::string WideStringToString(const std::wstring& wstr);
int safeToIntExact(long long value);
std::vector<BYTE> _IntToBytes(DWORD64 num, int lenght);
template<typename T>
std::vector<unsigned char> _IntToBytes(T data);
std::vector<BYTE>  IntToBytes(DWORD64 num);
std::vector<BYTE> 取空白ByteArr(int num);
long long int GetTime();
template<typename T>
std::vector<unsigned char> _IntToBytes(T data) {
    size_t size = sizeof(T);
    std::vector<unsigned char> ret(size);
    memcpy(ret.data(), &data, size);
    return ret;
}
std::vector<BYTE> 倒转ByteArr(std::vector<BYTE> bytes);
std::vector<int64_t> 取空白Arr(int num);
std::vector<BYTE> AddBytes(std::vector<BYTE> oldBytes, std::vector<BYTE> newBytes,std::vector<BYTE> newBytes2);
std::vector<BYTE>  AddBytes(std::vector<BYTE>  oldBytes, std::vector<BYTE>  newBytes);
std::vector<BYTE>  AnsiToUnicode(std::string str);




