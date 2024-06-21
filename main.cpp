//
// Created by 17129 on 2024/6/14.
//
#include "GameMove.h"
#include "GameMap.h"
#include "MapData.h"

int main() {
    GameMap gameMap;
    while (true) {
        std::vector<Coordinate> items = mapData.GetItemData();
        for (int i = 0; i < items.size(); ++i) {
            gameMap.move(items[i].x, items[i].y, 2);
        }
        gameMap.passMap();
        Sleep(50);
    }
}
//    std::cout << "Value: " << prt <<