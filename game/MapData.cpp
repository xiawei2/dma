//
// Created by 17129 on 2024/6/14.
//

#include "MapData.h"
Position GetCutRoom(){
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号)+时间基址)+门型偏移);
    return Position{
            mem.readInt(roomData+当前房间X),
            mem.readInt(roomData+当前房间Y),
            0
    };
}

Position GetBossRoom() {
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号)+时间基址)+门型偏移);
    return Position{
            mem.readInt(roomData+BOSS房间X),
            mem.readInt(roomData+BOSS房间Y),
            0
    };
}
