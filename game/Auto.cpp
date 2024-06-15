//
// Created by 17129 on 2024/6/15.
//

#include "Auto.h"

void passMap() {
    /**
     * 过图处理
     */

        if (!IsOpenDoor() || IsBossRoom()) {
            return;
        }

//        int overTheMap = iniUtils.read("自动配置", "过图方式", Integer.class);
//        if (overTheMap <= 0) {
//            return;
//        }

        MapDataType mapDataType = GetMapDataType();
        int direction = GetDirection(mapDataType.mapRoute[0], mapDataType.mapRoute[1]);
        if (direction < 0) {
            LOG("方向错误");
            return;
        }

//        switch (overTheMap) {
//            case 1:
//                gamecall.overMapCall(direction);
//            case 2:
//                gamecall.driftHandle(direction);
//        }

}
