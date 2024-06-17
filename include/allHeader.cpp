//
// Created by 17129 on 2024/6/17.
//
#include "allHeader.h"
Coordinate DNFCommon::GetBossRoom() {
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号) + 时间基址) + 门型偏移);
    return Coordinate{
            mem.readInt(roomData + BOSS房间X),
            mem.readInt(roomData + BOSS房间Y),
            0
    };
}


Coordinate DNFCommon::GetCutRoom() {
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号) + 时间基址) + 门型偏移);
    return Coordinate{
            mem.readInt(roomData + 当前房间X),
            mem.readInt(roomData + 当前房间Y),
            0
    };
}
int64_t DNFCommon::GetPersonPtr() {
    return mem.readLong(人物基址B);
}

bool DNFCommon::IsBossRoom() {
    auto position = GetCutRoom();
    auto bossPosition = GetBossRoom();
    return ArePointsEqual(position, bossPosition);
}
//
// Created by 17129 on 2024/6/14.
//

int DNFCommon::GetGameStat() {
    return mem.Read<int>(游戏状态);

}

int DNFCommon::GetPL() {
    return decode(最大疲劳) - decode(当前疲劳);
}

bool DNFCommon::IsTown() {
    return mem.readInt(GetPersonPtr() + 地图偏移);
}



bool DNFCommon::IsOpenDoor() {

    auto ptr = GetPersonPtr();
    auto encodedata = mem.readLong(mem.readLong(ptr +地图偏移-8) + 16);
    return decode(encodedata + 是否开门) == 0;
}



bool DNFCommon::ContainsItem() {
    return false;
}

bool DNFCommon::FtilterItem(std::string ItemName) {
    return false;
}



bool DNFCommon::ContainsMonster() {
    return false;
}


int DNFCommon::GetLevel() {
    return mem.readInt(角色等级);
}



//INT DNFCommon::GetGold() {
////    return decode(mem.readLong(mem.readLong(背包基址)+背包偏移));
//}

INT DNFCommon::GetItemNum(int ItemId) {
    return 0;
}

std::wstring GetMapName() {
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号) + 时间基址) + 门型偏移);
    auto data = mem.readBytes(mem.readLong(roomData + 地图名称), 52);
    return UnicodeToAnsi(data);
}

bool DNFCommon::IsPass() {
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号) + 时间基址) + 门型偏移);
    auto dataVal = mem.readInt(roomData + 篝火判断);
    return dataVal == 0 || dataVal == 2;
}

int DNFCommon::GetRoleLevel() {
    return mem.readInt(角色等级);
}
int DNFCommon::GetFame(){
    return mem.readInt(GetPersonPtr() + 人物名望);
}
INT DNFCommon::GetBagWeight(){
    auto personPtr = GetPersonPtr();
    auto data = mem.readLong(personPtr + 物品栏);
    int cutWeight = decode(data  +0x58);
    int maxWeight = decode(data + 最大负重);
    float result = (float)cutWeight / (float)maxWeight*100;
    return (int)result;
}
INT DNFCommon::GetPersonState(){
    return mem.readInt(GetPersonPtr() + 动作ID);
}
INT DNFCommon::decode(int64_t address) {
    return mem.Read<int>(address);
}
/**
 * 获取人物坐标
 * @return
 */
Coordinate DNFCommon::GetRolePosition() {
    return GetPosition(GetPersonPtr());
}
Coordinate DNFCommon::GetPosition(long long int ptr) {
    if (mem.readInt(ptr + 类型偏移) == 273) {
        auto data = mem.readLong(ptr + 读取坐标);
        return Coordinate(mem.readFloat(data), mem.readFloat(data + 4), mem.readFloat(data + 8));
    } else {
        auto data = mem.readLong(ptr + 方向偏移);
        return Coordinate(mem.readFloat(data + 32), mem.readFloat(data + +36), mem.readFloat(data + 40));
    }
}
