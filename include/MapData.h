//
// Created by 17129 on 2024/6/14.
//

#ifndef DMA_MAPDATA_H
#define DMA_MAPDATA_H

#include "allHeader.h"
#include "Coordinate.h"
#include "allHeader.h"

class MapData : public DNFCommon {
public:
/**
 * 取遍历指针
 *
 * @param ptr    long 指针地址
 * @param offset int 漂移计次
 * @param t      int  1 物品 2 地图
 * @return long
 */
    int64_t GetTraversalPtr(long ptr, long offset, int t);

/**
     * 地图遍历数据
     *
     * @return MapTraversalType
     */
    MapTraversalType GetMapData();

/**
 * 怪物遍历数据
 * @return
 */
    vector<Coordinate> GetMonsterData();

/**
 * 物品遍历数据
 * @return
 */
    vector<Coordinate> GetItemData();

    Coordinate GetPosition(long long int ptr);

    Coordinate GetNearestPosition(Coordinate position, vector<Coordinate> positions);

    int64_t GetTraversalPtr(int64_t ptr, int64_t offset, int t);

    int ComputedRange(Coordinate position, Coordinate position1);

    Coordinate GetRolePosition();

    void passMapByRun(int direction);
};

inline MapData mapData;
#endif //DMA_MAPDATA_H
