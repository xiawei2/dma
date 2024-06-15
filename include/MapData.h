//
// Created by 17129 on 2024/6/14.
//

#ifndef DMA_MAPDATA_H
#define DMA_MAPDATA_H
#include "Position.h"

class MapData {

};
Position GetCutRoom();
Position GetBossRoom();
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
vector<Position> GetMonsterData();
/**
 * 物品遍历数据
 * @return
 */
vector<Position> GetItemData();
Position GetPosition(long long int ptr);
Position GetNearestPosition(Position position, vector<Position> positions);
#endif //DMA_MAPDATA_H
