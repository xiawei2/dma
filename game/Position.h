//
// Created by 17129 on 2024/6/14.
//
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<wchar.h>
#include<locale.h>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <cstring>
#include <io.h>
#include <shlobj.h>
#include <tlhelp32.h>
#include "../nt/structs.h"
#include <Psapi.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <tchar.h>
#include <nb30.h>
#include<process.h>
#include<stdint.h>
#pragma comment(lib, "netapi32.lib")
#include <windows.h>
#include <string>
#include <fstream>
#include <time.h>
#include "address.h"
#include "../Memory/Memory.h"
#include "utils.h"
using namespace std;
#ifndef DMA_Position_H
#define DMA_Position_H


class Position {
public:
    Position(int i=0, int i1=0,int i2=0);

    int z;
    int y;
    int x;
};
int decode(int64_t address);

#endif //DMA_坐标型_H
