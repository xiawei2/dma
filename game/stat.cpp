//
// Created by 17129 on 2024/6/14.
//
#include "stat.h"

int GetGameStat()
{
    return mem.Read<int>( 游戏状态);

}

int GetPL() {
    return decode(最大疲劳)- decode(当前疲劳);
}

bool IsTown() {
    return mem.readInt(GetPersonPrt()+地图偏移);
}

int64_t GetPersonPrt() {
    return 人物基址B;
}

bool IsOpenDoor() {
    auto ptr = GetPersonPrt();
    auto encodedata = mem.readInt(ptr + 地图偏移);
    return decode(encodedata+是否开门) ==0;
}

bool IsBossRoom() {
    auto position =GetCutRoom();
    auto bossPosition = GetBossRoom();
    return ArePointsEqual(position,bossPosition);
    return false;
}

bool ContainsItem() {
    return false;
}

bool FtilterItem(std::string ItemName) {
    return false;
}

bool ArePointsEqual(Position index1, Position index2) {
    if (index1.z == index2.z&&index1.x == index2.x&&index1.y == index2.y){
        return true;
    }
    return false;
}

bool ContainsMonster() {
    return false;
}

int GetLevel() {
    return 0;
}

Position GetPosition(long long int ptr) {
    return Position();
}

INT GetGold() {
    return 0;
}

INT GetItemNum(int ItemId) {
    return 0;
}

std::wstring GetMapName() {
    long roomData = mem.readLong(mem.readLong(mem.readLong(房间编号)+时间基址)+门型偏移);
    auto data =mem.readBytes(mem.readLong(roomData+地图名称),52);

    return UnicodeToAnsi(data);
}

bool IsPass() {
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号)+时间基址)+门型偏移);
    auto dataVal = mem.readInt(roomData+篝火判断);
    return dataVal == 0|| dataVal == 2;
}

int GetRoleLevel() {
    return mem.readInt(角色等级);
}

