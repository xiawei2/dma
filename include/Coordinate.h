//
// Created by 17129 on 2024/6/14.
//

#define _CRT_SECURE_NO_WARNINGS
#pragma once
#ifndef DMA_Position_H
#define DMA_Position_H
#include <vector>
#include <cstdint>
#include <string>

class Coordinate {
public:
    Coordinate(int i=0, int i1=0, int i2=0);
    int x;
    int y;
    int z;
    // 重载==运算符
    bool operator==(const Coordinate& other) const {
        // 根据Coordinate的定义，比较成员变量判断两个Coordinate是否相等
        // 例如，如果Coordinate有两个成员x和y，比较方式可能是：
        return x == other.x && y == other.y && z == other.z;
    }
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
    Coordinate rwCoordinate;// 地图坐标类型
    Coordinate gwCoordinate;// 怪物坐标
    Coordinate wpCoordinate;// 终点坐标类型
};
class MapDataType{
public:
    std::wstring mapName;// 地图名称
    int mapId; // 地图编号
    std::vector<int> mapChannel;// 地图通道
    Coordinate startCoordinate;// 起始坐标
    Coordinate endCoordinate;// 终点坐标
    int width;// 宽
    int height;// 高
    std::vector<Coordinate> mapRoute;// 地图走法
    int consumeFatigue;// 消耗疲劳
    int channelNum; // 通道数量
    int64_t tmp;// 临时变量
    MapDataType(){
        mapName = L"";
        mapId = 0;
        mapChannel = {};
        startCoordinate = Coordinate();
        endCoordinate = Coordinate();
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
    Coordinate mapCoordinate;// 地图坐标
    bool left; // 地图左边
    bool right;// 地图右边
    bool up;// 地图上边
    bool down;// 地图下边
    int mapChannel;// 地图通道
    int backgroundColor;// 地图背景颜色
    GameMapType(){
        mapCoordinate = Coordinate();
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
    Coordinate currentCoordinates;// 当前坐标
    Coordinate finalCoordinates;// 最终坐标
    MapNodeType(){
        f = 0;
        g = 0;
        h = 0;
        currentCoordinates = Coordinate();
        finalCoordinates = Coordinate();
    }
};




//VncViewer viewer = VncViewer();
#define XK_Left			0xFF51	/* Move left, left arrow */
#define XK_Up			0xFF52	/* Move up, up arrow */
#define XK_Right		0xFF53	/* Move right, right arrow */
#define XK_Down			0xFF54	/* Move down, down arrow */
//int decode(int64_t address);

#endif //DMA_坐标型_H
