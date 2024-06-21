//
// Created by 17129 on 2024/6/14.
//

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
class utils {

};


#endif //DMA_UTILS_H
std::wstring UnicodeToAnsi(std::vector<BYTE> bytes);
bool ArePointsEqual(Coordinate index1, Coordinate index2);

int rnd(int min, int max);
void split(const std::wstring& str, std::vector<std::wstring>& tokens, const std::wstring& delimiters);
std::string WideStringToString(const std::wstring& wstr);
int safeToIntExact(long long value);



