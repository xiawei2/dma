//
// Created by 17129 on 2024/6/16.
//

#ifndef DMA_ALLHEADER_H
#define DMA_ALLHEADER_H
#pragma once
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<wchar.h>
#include<locale.h>
#include <tchar.h>

#include <string>
#include <cstring>
#include <io.h>
#include <shlobj.h>
#include <tlhelp32.h>
#include "structs.h"
#include <psapi.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <tchar.h>
#include <nb30.h>
#include<process.h>
#include<cstdint>
#include <vector>
#include "Memory.h"
#include "utils.h"
//#include "GameMove.h"
#pragma comment(lib, "netapi32.lib")
#include "Coordinate.h"
//#include <windows.h>
#include <string>
#include <fstream>
#include <ctime>
#include "address.h"
using namespace std;
#endif //DMA_ALLHEADER_H
class DNFCommon{
public:
    Coordinate GetBossRoom();
    Coordinate GetCutRoom();
    int64_t GetPersonPtr();
    bool IsOpenDoor();
    bool IsBossRoom();
    /**
* 游戏状态
*
* @return int 0选角 1城镇 2选图 3图内 5选择频道
*/
    int GetGameStat();

    int GetPL();
/**
  * 是否城镇
  *
  * @return boolean
  */
    bool IsTown();
    int decode(int64_t address);


    bool FtilterItem(std::string ItemName);

    bool ContainsItem();



    bool ContainsMonster();


    int GetLevel();

    bool IsPass();


    int GetRoleLevel();

    INT GetGold();

    INT GetItemNum(int ItemId);

    std::wstring GetMapName();
/**
 * 获取名望
 * @return
 */
    INT GetFame();
/**
 * 获取背包负重
 */
    INT GetBagWeight();
//0站立、6跳跃、8普攻、14跑动
    INT GetPersonState();

    Coordinate GetRolePosition();

    static Coordinate GetPosition(long long int ptr);
    bool GetPersonItem();
};









