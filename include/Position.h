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
#include <psapi.h>
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

#include "../Memory/Memory.h"
#include "utils.h"

using namespace std;
#ifndef DMA_Position_H
#define DMA_Position_H


class Position {
public:
    Position(int i=0, int i1=0,int i2=0);
    int x;
    int y;
    int z;
};
class MapTraversalType {
public:
    int64_t rwAddr{};// 地图地址
    int64_t mapData{};// 地图数据
    int64_t start{};// 起始位置
    int64_t end{};// 结束位置
    int64_t objNum{};//物体数量
    int64_t objTmp{};// 物体临时变量
    int64_t objPtr{};// 物体指针
    int64_t objCamp{};// 物体阵营
    int64_t objBlodd{};// 物体血量
    int64_t objTypeA{};// 物体类型A
    int64_t objTypeB{};// 物体类型B
    int64_t objCode{};// 物体代码
    int64_t objNameA{};// 物体名称A
    int64_t objNameB{};// 物体名称B
    Position rwCoordinate;// 地图坐标类型
    Position gwCoordinate;// 怪物坐标
    Position wpCoordinate;// 终点坐标类型
};
class MapDataType{
public:
    wstring mapName;// 地图名称
    int mapId; // 地图编号
    vector<int> mapChannel;// 地图通道
    Position startCoordinate;// 起始坐标
    Position endCoordinate;// 终点坐标
    int width;// 宽
    int height;// 高
    vector<Position> mapRoute;// 地图走法
    int consumeFatigue;// 消耗疲劳
    int channelNum; // 通道数量
    int64_t tmp;// 临时变量
    MapDataType(){
        mapName = L"";
        mapId = 0;
        mapChannel = {};
        startCoordinate = Position();
        endCoordinate = Position();
        width = 0;
        height = 0;
        mapRoute = {};
        consumeFatigue = 0;
        channelNum = 0;
        tmp = 0;
    }
};
class GameMapType{
public:
    Position mapCoordinate;// 地图坐标
    bool left; // 地图左边
    bool right;// 地图右边
    bool up;// 地图上边
    bool down;// 地图下边
    int mapChannel;// 地图通道
    int backgroundColor;// 地图背景颜色
    GameMapType(){
        mapCoordinate = Position();
        left = false;
        right = false;
        up = false;
        down = false;
        mapChannel = 0;
        backgroundColor = 0;
    }
};
class MapNodeType{
public:
    int f; // 地图F点
    int g; // 地图G点
    int h; // 地图H点
    Position currentCoordinates;// 当前坐标
    Position finalCoordinates;// 最终坐标
    MapNodeType(){
        f = 0;
        g = 0;
        h = 0;
        currentCoordinates = Position();
        finalCoordinates = Position();
    }
};
int decode(int64_t address);
std::string WideStringToString(const std::wstring& wstr);
int safeToIntExact(long long value);
#endif //DMA_坐标型_H
