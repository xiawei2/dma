//
// Created by 17129 on 2024/6/15.
//

#ifndef DMA_AUTO_H
#define DMA_AUTO_H
#include "GameMove.h"
#include "GameMap.h"
#include "MapData.h"
class GameAuto {
public:
    GameMap gameMap;
    void Fight();
    void Finish();
    void Start();
private:
    int times = 0;// 通关次数
    bool stat = true;// 运行状态
};
//void passMap();

#endif //DMA_AUTO_H
