//
// Created by 17129 on 2024/6/14.
//

#ifndef DMA_GAMEMAP_H
#define DMA_GAMEMAP_H
#include "allHeader.h"
#include "Coordinate.h"
#include "map"
#include "GameMove.h"



#endif //DMA_GAMEMAP_H
class GameMap: public DNFCommon{
public:
    long long int speed;
    int SkillList[14] = {XK_A, XK_S, XK_D, XK_F, XK_G, XK_H, XK_Alt_L,XK_Q, XK_W, XK_E, XK_R, XK_T, XK_Y,XK_Control_L};
    std::map<int, long long int > SkillMap;
    std::vector<Coordinate> path;// 当前房间是否使用过大技能
    int SkillBig[5] = {8, 9, 10, 11, 12};
    void InitSkillMap();
    MapDataType GetMapDataType();

    int GetRoute(std::vector<int> mapChannel, int width, int height, Coordinate start, Coordinate end,
                 std::vector<Coordinate> &mapRoute);

/**
 * 生成地图
 * @param width 宽
 * @param height 高
 * @param mapChannel 地图通道
 */
    std::vector<std::vector<GameMapType>> GenMap(int width, int height, std::vector<int> mapChannel);

/**
 * 显示地图
 *
 * @param mapArr 地图数组
 * @param width  宽
 * @param height 高
 * @return 游戏地图
 */
    std::vector<std::vector<GameMapType>>
    DisplayMap(std::vector<std::vector<GameMapType>> mapArr, int width, int height);

/**
 * 路径计算
 * @param mapLabel 地图标签
 * @param mapStart 地图起始
 * @param mapEnd 地图终止
 * @param width 宽
 * @param height 高
 * @return
 */
    std::vector<Coordinate>
    RouteCalculate(std::vector<std::vector<GameMapType>> mapLabel, Coordinate mapStart, Coordinate mapEnd, int width,
                   int height);

/**
 * 获取方向
 *
 * @param cutRoom  当前房间
 * @param nextRoom 下一个房间
 * @return 方向
 */
    int GetDirection(Coordinate curtRoom, Coordinate nextRoom);

    MapNodeType GetMapNode(Coordinate mapEnd, Coordinate waitHandleCoordinate, MapNodeType tmpNode);

/**
 * 整理坐标
 * @param simulationRoute 模拟路线
 * @param realityRoute    实际路线
 * @return 消耗疲劳
 */
    int TidyCoordinate(std::vector<Coordinate> simulationRoute, std::vector<Coordinate> &realityRoute);

/**
 * 寻路判断方向
 * @param tx 通向
 * @param fx 方向
 * @return 是否可走
 */
    bool JudgeDirection(int tx, int fx);
    double GetSpeed();

    double GetMoveSpeed();

    void passMapByRun(int direction);


    void passMap();

    void move(int x1, int y1, int type);

    bool PickItem();
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

//    Coordinate GetPosition(long long int ptr);

    Coordinate GetNearestPosition(Coordinate position, vector<Coordinate> positions, int *pInt);

    int64_t GetTraversalPtr(int64_t ptr, int64_t offset, int t);

    int ComputedRange(Coordinate position, Coordinate position1);

//    Coordinate GetRolePosition();

    void AttactMonster();

    void UnleashSkill();


    vector<int> GetCanSkills();

    bool IsPass();
    // 0 未翻牌
    //  1 翻牌完成在图内
    //  4 翻牌中

    int 取翻牌状态();
};
inline GameMap gameMap;

