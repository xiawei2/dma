//
// Created by 17129 on 2024/6/14.
//

#ifndef DMA_GAMEMAP_H
#define DMA_GAMEMAP_H

#include "Position.h"
#include "MapData.h"
class GameMap {

};

MapDataType GetMapDataType();
int GetRoute(std::vector<int> mapChannel,int width,int height,Position start,Position end,std::vector<Position> &mapRoute);
/**
 * 生成地图
 * @param width 宽
 * @param height 高
 * @param mapChannel 地图通道
 */
std::vector<std::vector<GameMapType>> GenMap(int width,int height,std::vector<int> mapChannel);
/**
 * 显示地图
 *
 * @param mapArr 地图数组
 * @param width  宽
 * @param height 高
 * @return 游戏地图
 */
std::vector<std::vector<GameMapType>> DisplayMap(std::vector<std::vector<GameMapType>> mapArr,int width,int height);
/**
 * 路径计算
 * @param mapLabel 地图标签
 * @param mapStart 地图起始
 * @param mapEnd 地图终止
 * @param width 宽
 * @param height 高
 * @return
 */
std::vector<Position> RouteCalculate(std::vector<std::vector<GameMapType>> mapLabel,Position mapStart ,Position mapEnd,int width,int height);
/**
 * 获取方向
 *
 * @param cutRoom  当前房间
 * @param nextRoom 下一个房间
 * @return 方向
 */
int GetDirection(Position curtRoom,Position nextRoom);
MapNodeType GetMapNode(Position mapEnd,Position waitHandleCoordinate,MapNodeType tmpNode);
/**
 * 整理坐标
 * @param simulationRoute 模拟路线
 * @param realityRoute    实际路线
 * @return 消耗疲劳
 */
int TidyCoordinate(std::vector<Position> simulationRoute,std::vector<Position> &realityRoute);
/**
 * 寻路判断方向
 * @param tx 通向
 * @param fx 方向
 * @return 是否可走
 */
bool JudgeDirection(int tx,int fx);
#endif //DMA_GAMEMAP_H
