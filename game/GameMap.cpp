//
// Created by 17129 on 2024/6/14.
//
#include "address.h"
#include "GameMap.h"


MapDataType GetMapDataType() {
    MapDataType data =  MapDataType();
    //(房间编号)+时间地址)+
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号) +时间基址) + 门型偏移);
    auto roomIndex = decode(roomData +索引偏移);

    data.width = mem.readInt(mem.readLong(roomData + 宽高偏移) + roomIndex * 8);
    data.height = mem.readInt(mem.readLong(roomData + 宽高偏移) + roomIndex * 8 + 4);
    data.tmp = mem.readLong(mem.readLong(roomData + 数组偏移) + 32 * roomIndex + 8);
    data.channelNum = data.width * data.height;
    for (int i = 0; i < data.channelNum; i++) {
        data.mapChannel.insert(data.mapChannel.begin()+i, mem.readInt(data.tmp + i * 4L));
    }
    auto cutRoom = GetCutRoom();
    auto bossRoom = GetBossRoom();
    data.startCoordinate.x =  cutRoom.x+ 1;
    data.startCoordinate.y = cutRoom.y + 1;
    data.endCoordinate.x = bossRoom.x + 1;
    data.endCoordinate.y = bossRoom.y + 1;

    if (data.startCoordinate.x == data.endCoordinate.x && data.startCoordinate.y == data.endCoordinate.y) {
        return data;
    }

    data.consumeFatigue = GetRoute(data.mapChannel, data.width, data.height, data.startCoordinate, data.endCoordinate, data.mapRoute);
    return data;
}

int GetRoute(vector<int> mapChannel, int width, int height, Position mapStart, Position mapEnd,
             vector<Position> &mapRoute) {
    Position startCoordinate;  // 起始坐标
    Position endCoordinate; // 结束坐标
    if (mapStart.x == mapEnd.x && mapStart.x == mapEnd.y) {
        mapRoute.clear();
        mapRoute.resize(0);
        return 0;
    }
    // 地图数组
    vector<vector<GameMapType>> mapArray = GenMap(width, height, mapChannel);
    // 标签数组
    vector<vector<GameMapType>> mapFlag = DisplayMap(mapArray, width, height);

    startCoordinate.x = mapStart.x * 3 - 2;
    startCoordinate.y = mapStart.y * 3 - 2;
    endCoordinate.x = mapEnd.x * 3 - 2;
    endCoordinate.y = mapEnd.y * 3 - 2;
    vector<Position> crossWay = RouteCalculate(mapFlag, startCoordinate, endCoordinate, width * 3, height * 3);
    return TidyCoordinate(crossWay, mapRoute);
}

std::vector<std::vector<GameMapType>> GenMap(int width, int height, vector<int> mapChannel) {
    std::vector<std::vector<GameMapType>> gameMap(width, std::vector<GameMapType>(height));
    int i = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            gameMap[x][y].mapCoordinate.x = x;
            gameMap[x][y].mapCoordinate.y = y;
            gameMap[x][y].mapChannel = mapChannel[i];
            gameMap[x][y].left = JudgeDirection(mapChannel[i], 0);
            gameMap[x][y].right = JudgeDirection(mapChannel[i], 1);
            gameMap[x][y].up = JudgeDirection(mapChannel[i], 2);
            gameMap[x][y].down = JudgeDirection(mapChannel[i], 3);
            gameMap[x][y].backgroundColor = 0xFFFFFF;
            i++;
            if (gameMap[x][y].mapChannel == 0) {
                gameMap[x][y].backgroundColor = 0x000000;
            }

        }
    }
    return gameMap;
}

std::vector<std::vector<GameMapType>> DisplayMap(std::vector<std::vector<GameMapType>> mapArr, int width, int height) {
    std::vector<std::vector<GameMapType>> mapLabel(width * 3, std::vector<GameMapType>(height * 3));

    for (int x = 0; x < width * 3; x++) {
        for (int y = 0; y < height * 3; y++) {
            mapLabel[x][y] = GameMapType();
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            mapLabel[(x + 1) * 3 - 2][(y + 1) * 3 - 2].backgroundColor = 0xFFFFFF;
            if (mapArr[x][y].left) {
                mapLabel[(x + 1) * 3 - 3][(y + 1) * 3 - 2].backgroundColor = 0xFFFFFF;
            }
            if (mapArr[x][y].right) {
                mapLabel[(x + 1) * 3 - 1][(y + 1) * 3 - 2].backgroundColor = 0xFFFFFF;
            }
            if (mapArr[x][y].up) {
                mapLabel[(x + 1) * 3 - 2][(y + 1) * 3 - 3].backgroundColor = 0xFFFFFF;
            }
            if (mapArr[x][y].down) {
                mapLabel[(x + 1) * 3 - 2][(y + 1) * 3 - 1].backgroundColor = 0xFFFFFF;
            }
        }
    }
    return mapLabel;
}

std::vector<Position>
RouteCalculate(std::vector<std::vector<GameMapType>> mapLabel, Position mapStart, Position mapEnd, int width,
               int height) {
    MapNodeType tmpNode = MapNodeType(); // 待检测节点, 临时节点
    std::vector<MapNodeType> openList = std::vector<MapNodeType>(); // 开放列表
    std::vector<MapNodeType> closeList = std::vector<MapNodeType>(); // 关闭列表

    int shortEstNum = 0; // 最短编号

    tmpNode.currentCoordinates.x = mapStart.x;
    tmpNode.currentCoordinates.y = mapStart.y;
    mapLabel[safeToIntExact(mapStart.x)][safeToIntExact(mapStart.y)].backgroundColor = 0x00FF00;
    mapLabel[safeToIntExact(mapStart.x)][safeToIntExact(mapStart.y)].backgroundColor = 0x0000FF;
    openList.insert(openList.begin(), tmpNode);

    std::vector<Position> moveArr = std::vector<Position>();

    do {
        int minF = 0;
        for (int y = 0; y < openList.size(); y++) {
            if (minF == 0) {
                minF = openList[0].f;
                shortEstNum = y;
            }
            if (openList[y].f < minF) {
                minF = openList[y].f;
                shortEstNum = y;
            }
        }

        tmpNode = openList[shortEstNum];
        openList.erase(openList.begin() + shortEstNum);
        closeList.insert(closeList.begin(), tmpNode);

        if (tmpNode.currentCoordinates.x != mapStart.x || tmpNode.currentCoordinates.y != mapStart.y) {
            if (tmpNode.currentCoordinates.x != mapEnd.x || tmpNode.currentCoordinates.y != mapEnd.y) {
                mapLabel[safeToIntExact(tmpNode.currentCoordinates.x)][safeToIntExact(
                        tmpNode.currentCoordinates.y)].backgroundColor = 0x0080FF;
            }
        }
        for (int y = 0; y < closeList.size(); y++) {
            if (closeList[y].currentCoordinates.x == mapEnd.x &&
                closeList[y].currentCoordinates.y == mapEnd.y) {
                MapNodeType waitHandleNode = closeList[y];
                do {
                    for (MapNodeType mapNodeType: closeList) {
                        if (mapNodeType.currentCoordinates.x == waitHandleNode.finalCoordinates.x &&
                            mapNodeType.currentCoordinates.y == waitHandleNode.finalCoordinates.y) {
                            waitHandleNode = mapNodeType;
                            break;
                        }
                    }
                    if (waitHandleNode.currentCoordinates.x != mapStart.x ||
                        waitHandleNode.currentCoordinates.y != mapStart.y) {
                        mapLabel[safeToIntExact(waitHandleNode.currentCoordinates.x)][safeToIntExact(
                                waitHandleNode.currentCoordinates.y)].backgroundColor = 0x00D8D8;
                        moveArr.insert(moveArr.begin(), waitHandleNode.currentCoordinates);
                    }

                } while (waitHandleNode.currentCoordinates.x != mapStart.x ||
                         waitHandleNode.currentCoordinates.y != mapStart.y);
                moveArr.insert(moveArr.begin(), mapStart);
                moveArr.insert(moveArr.end(),mapEnd);
                return moveArr;
            }
        }
        for (int y = 0; y < 4; y++) {
            Position waitHandleCoordinate =  Position(); // 待检测坐标
            if (y == 0) {
                waitHandleCoordinate.x = tmpNode.currentCoordinates.x;
                waitHandleCoordinate.y = tmpNode.currentCoordinates.y - 1;
            } else if (y == 1) {
                waitHandleCoordinate.x = tmpNode.currentCoordinates.x - 1;
                waitHandleCoordinate.y = tmpNode.currentCoordinates.y;
            } else if (y == 2) {
                waitHandleCoordinate.x = tmpNode.currentCoordinates.x + 1;
                waitHandleCoordinate.y = tmpNode.currentCoordinates.y;
            } else {
                waitHandleCoordinate.x = tmpNode.currentCoordinates.x;
                waitHandleCoordinate.y = tmpNode.currentCoordinates.y + 1;
            }
            if (waitHandleCoordinate.x<0 || waitHandleCoordinate.x>(width - 1) ||
                waitHandleCoordinate.y<0 || waitHandleCoordinate.y>(height - 1)) {
                continue;
            }
            if (mapLabel[safeToIntExact(waitHandleCoordinate.x)][safeToIntExact(
                    waitHandleCoordinate.y)].backgroundColor == 0x000000) {
                continue;
            }
            boolean existCloseList = false;
            for (MapNodeType nodeType: closeList) {
                if (nodeType.currentCoordinates.x == waitHandleCoordinate.x &&
                    nodeType.currentCoordinates.y == waitHandleCoordinate.y) {
                    existCloseList = true;
                    break;
                }
            }
            if (existCloseList) {
                continue;
            }
            boolean existOpenList = false;
            for (MapNodeType mapNodeType: openList) {
                if (mapNodeType.currentCoordinates.x == waitHandleCoordinate.x &&
                    mapNodeType.currentCoordinates.y == waitHandleCoordinate.y) {
                    int guessG;
                    if (waitHandleCoordinate.x != tmpNode.currentCoordinates.x ||
                        waitHandleCoordinate.y != tmpNode.currentCoordinates.y) {
                        guessG = 14;
                    } else {
                        guessG = 10;
                    }

                    if (tmpNode.g + guessG < mapNodeType.g) {
                        mapNodeType.finalCoordinates=tmpNode.currentCoordinates;
                    }

                    existOpenList = true;
                    break;
                }
            }
            if (!existOpenList) {
                MapNodeType waitHandleNode = GetMapNode(mapEnd, waitHandleCoordinate, tmpNode);
                openList.insert(openList.begin(), waitHandleNode);
            }

        }
    } while (openList.size()!=0);
    return moveArr;
}

int GetDirection(Position curtRoom, Position nextRoom) {
    int direction = 0;
    int x = curtRoom.x - nextRoom.x;
    int y = curtRoom.y - nextRoom.y;
    if (x == 0 && y == 0) {
        return 4;
    }
    if (x == 0) {
        if (y == 1) {
            direction = 2;
        } else {
            direction = 3;
        }
    } else if (y == 0) {
        if (x == 1) {
            direction = 0;
        } else {
            direction = 1;
        }
    }
    return direction;
}

MapNodeType GetMapNode(Position mapEnd, Position waitHandleCoordinate, MapNodeType tmpNode) {
    /* 预估从等待处理坐标到当前节点的直线距离，用于计算G值 */
    int guessG;
    /* 如果等待处理的坐标在当前节点的x或y轴上与当前节点相邻，则预估距离为10，否则为14 */
    if (waitHandleCoordinate.x == tmpNode.currentCoordinates.x || waitHandleCoordinate.y == tmpNode.currentCoordinates.y) {
        guessG = 10;
    } else {
        guessG = 14;
    }
    /* 创建一个新的节点类型对象，用于表示等待处理的节点 */
    MapNodeType waitHandleNode =  MapNodeType();
    /* 设置该节点的G值，即从起点到当前节点的预估费用 */
    waitHandleNode.g=(tmpNode.g + guessG);
    /* 设置该节点的H值，即从当前节点到目标节点的预估直线距离 */
    /* 这里使用了地图坐标与实际坐标之间的换算关系，将坐标乘以10以适应特定的坐标系统 */
    waitHandleNode.h=(safeToIntExact(safeToIntExact(mapEnd.x) - safeToIntExact(waitHandleCoordinate.x * 10L) + (mapEnd.y) - safeToIntExact(waitHandleCoordinate.y * 10L)));
    /* 设置该节点的F值，即G值和H值之和，用于A*算法中节点的排序 */
    waitHandleNode.f=(waitHandleNode.g + waitHandleNode.h);
    /* 设置该节点的当前坐标为等待处理的坐标 */
    waitHandleNode.currentCoordinates=(waitHandleCoordinate);
    /* 设置该节点的目标坐标为当前节点的坐标 */
    waitHandleNode.finalCoordinates=(tmpNode.currentCoordinates);
    /* 返回计算出的等待处理节点的类型 */
    return waitHandleNode;
}

int TidyCoordinate(std::vector<Position> simulationRoute, std::vector<Position> &realityRoute) {
    int x, y, k = 0;
    for (Position coordinateType : simulationRoute) {
        Position tempCoordinates = Position();
        x = (coordinateType.x + 2) % 3;
        y = (coordinateType.y + 2) % 3;
        if (x == 0 && y == 0) {
            tempCoordinates.x = (coordinateType.x + 2) / 3 - 1;
            tempCoordinates.y = (coordinateType.y + 2) / 3 - 1;
            realityRoute.insert(realityRoute.begin()+k, tempCoordinates);
            k++;
        }
    }
    return k;
}

bool JudgeDirection(int tx, int fx) {
    // 方向数组
    unsigned char directionArr[4];
    // 方向集合
    unsigned char directionSet[16][4] = {{0, 0, 0, 0},
                                         {0, 1, 0, 0},
                                         {0, 0, 1, 0},
                                         {0, 1, 1, 0},
                                         {1, 0, 0, 0},
                                         {1, 1, 0, 0},
                                         {1, 0, 1, 0},
                                         {1, 1, 1, 0},
                                         {0, 0, 0, 1},
                                         {0, 1, 0, 1},
                                         {0, 0, 1, 1},
                                         {0, 1, 1, 1},
                                         {1, 0, 0, 1},
                                         {1, 1, 0, 1},
                                         {1, 0, 1, 1},
                                         {1, 1, 1, 1}};

    if (tx >= 0 && tx <= 15) {
        for (int i = 0; i < 4; i++) {
            directionArr[i] = directionSet[tx][i];
        }
    } else {
        for (int i = 0; i < 4; i++) {
            directionArr[i] = 0;
        }
    }

    if (directionArr[fx] == 1) {
        return true;
    } else {
        return false;
    }
}
