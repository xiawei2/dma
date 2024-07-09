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
    auto ptr = mem.readLong(新人物基址)-48;
    if (ptr>0&&mem.readLong(人物基址B)==ptr){
        return ptr;
    }
    return 0;
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
    return mem.readInt(GetPersonPtr() + 地图偏移-8); // 7度不需要-8
}

bool DNFCommon::IsSkillFlush(int index) {
    auto skillPtr = GetPersonPtr();
    skillPtr = mem.readLong(skillPtr + 技能栏);
    skillPtr = mem.readLong(skillPtr + 技能栏偏移);
    skillPtr = mem.readLong(mem.readLong(skillPtr + index*24)+16)-16;
    return (GetFlushTime(skillPtr))==0;
}
int DNFCommon::GetSkillCD(int index) {
    auto skillPtr = GetPersonPtr();
    skillPtr = mem.readLong(skillPtr + 技能栏);
    skillPtr = mem.readLong(skillPtr + 技能栏偏移);
    skillPtr = mem.readLong(mem.readLong(skillPtr + index*24)+16)-16;
    auto time = GetFlushTime(skillPtr);
    auto lastKeyTime = mem.readInt(skillPtr+判断冷却_1);// 最后按键时间
    auto eax = mem.readInt(skillPtr+判断冷却_2);
    auto edi = mem.readInt(skillPtr+判断冷却_2 +4);
    auto xmm1 = mem.readFloat(skillPtr+判断冷却_2 +8);
    if (lastKeyTime==0){
        return -1;
    }
    return time;
}

bool DNFCommon::IsOpenDoor() {

    auto ptr = GetPersonPtr();
    auto encodedata = mem.readLong(mem.readLong(ptr +地图偏移-16) + 16); // 7度要-8
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
    int maxWeight = decode(personPtr + 最大负重);
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

bool DNFCommon::GetPersonItem() {
    return (mem.readLong(mem.readLong(人物空白地址) +脚下物品) > 0);
}

int DNFCommon::GetFlushTime(int64_t skillPtr) {
    auto 参数2 = 冷却参数_2-8;
    long long int lastKeyTime = mem.readInt(skillPtr+判断冷却_1);// 最后按键时间
    long long int eax = mem.readInt(skillPtr+判断冷却_2);
    long long int edi = mem.readInt(skillPtr+判断冷却_2 +4);
    auto xmm1 = mem.readFloat(skillPtr+判断冷却_2 +8);
    long long int ret = mem.readInt(冷却参数_1+mem.readInt(参数2+8)*4)+mem.readInt(参数2+16);
    ret = (ret -eax)*xmm1 + edi;
//   auto ret = mem.readInt(冷却参数_1);
    return ret - lastKeyTime>0?0:ret - lastKeyTime;
}

int DNFCommon::GetShop() {
    return mem.readInt(通关商店);
}

bool DNFCommon::IsJiaBaiLi() {
    auto shop = mem.readInt(通关商店);
    return (shop ==1002||shop ==1003);
}
void DNFCommon::武器冰冻(int64_t 伤害){
    bool static 开关=false;
    int64_t 冰冻地址 = 全局空白+4848;//+8范围  +12频率  +16触发时间  +20触发几率  +24冰冻几率  +28伤害
    if (!开关){
        mem.Write(mem.readLong(GetPersonPtr()+武器偏移)+冰冻开始, 冰冻地址);
        mem.Write(mem.readLong(GetPersonPtr()+武器偏移)+冰冻结束,冰冻地址+32);
        mem.Write(冰冻地址+8,3000);// 范围
        mem.Write(冰冻地址+12,100);// 频率
        mem.Write(冰冻地址+28,伤害);// 伤害
        LOG("冰冻开启\n");
    }else{
        mem.Write(mem.readLong(GetPersonPtr()+武器偏移)+冰冻开始, 0);
        mem.Write(mem.readLong(GetPersonPtr()+武器偏移)+冰冻结束, 0);
        LOG("冰冻关闭\n");
    }
    开关=!开关;
}
void DNFCommon::无视建筑(){
    bool static 穿透=false;
    int64_t 穿透地址 = 全局空白+4848;//+8范围  +12频率  +16触发时间  +20触发几率  +24冰冻几率  +28伤害
    if (!穿透){
        // 无视建筑
        mem.Write(GetPersonPtr() + 建筑穿透, 0);
        mem.Write(GetPersonPtr() + 地图穿透, 0);
    }else{

        // 还原
        mem.Write(GetPersonPtr()+建筑穿透,40);
        mem.Write(GetPersonPtr()+地图穿透,10);
    }
}


