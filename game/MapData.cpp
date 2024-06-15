//
// Created by 17129 on 2024/6/14.
//
#include <complex>
#include "address.h"
#include "MapData.h"


int ComputedRange(Position position, Position position1);

Position GetCutRoom() {
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号) + 时间基址) + 门型偏移);
    return Position{
            mem.readInt(roomData + 当前房间X),
            mem.readInt(roomData + 当前房间Y),
            0
    };
}

Position GetBossRoom() {
    auto roomData = mem.readLong(mem.readLong(mem.readLong(房间编号) + 时间基址) + 门型偏移);
    return Position{
            mem.readInt(roomData + BOSS房间X),
            mem.readInt(roomData + BOSS房间Y),
            0
    };
}

MapTraversalType GetMapData() {
    MapTraversalType result = MapTraversalType();
    result.rwAddr = mem.readLong(人物空白地址);
    result.mapData = mem.readLong(mem.readLong(result.rwAddr + 地图偏移 - 8) + 16);
    result.start = mem.readLong(result.mapData + 地图开始2);
    result.end = mem.readLong(result.mapData + 地图结束2);
    result.objNum = (result.end - result.start) / 24;
    return result;
}

int64_t GetTraversalPtr(int64_t ptr, int64_t offset, int t) {
    int64_t result = 0;
    if (t == 1) {
        auto one = mem.readLong(ptr + (offset - 1) * 8);
        auto two = mem.readLong(one - 72);
        result = mem.readLong(two + 16);
    }
    if (t == 2) {
        auto one = mem.readLong(ptr + (offset - 1) * 24);
        result = mem.readLong(one + 16) - 48;
    }
    return result;
}

vector<Position> GetMonsterData() {
    MapTraversalType data = GetMapData();
    vector<Position> result = vector<Position>();
    for (data.objTmp = 1; data.objTmp < data.objNum; data.objTmp++) {
        data.objPtr = GetTraversalPtr(data.start, data.objTmp, 2);
        if (data.objPtr > 0) {
            data.objTypeA = mem.readInt(data.objPtr + 类型偏移);
            if (data.objTypeA == 529 || data.objTypeA == 545 || data.objTypeA == 273 || data.objTypeA == 61440 ||
                data.objTypeA == 1057) {
                // 阵营
                data.objCamp = mem.readInt(data.objPtr + 阵营偏移);
                // 物体代码
                data.objCode = mem.readInt(data.objPtr + 代码偏移);
                // 物体血量
                data.objBlodd = mem.readInt(data.objPtr + 怪物血量);
                if (data.objCamp > 0 && data.objPtr != data.rwAddr) {
                    if (data.objBlodd > 0) {
                        result.insert(result.end(), GetPosition(data.objPtr));
                    }
                }
            }
        }
    }
    return result;
}

vector<Position> GetItemData() {
    std:
    wstring filterItems = L"橙子-哈密瓜-草莓-碎布片-生锈的铁片-破旧的皮革-最下级砥石-最下级硬化剂-风化的碎骨-入门HP药剂-入门MP药剂-普通HP药剂-普通MP药剂-肉干-命运硬币-轰爆弹-爆弹-燃烧瓶-精灵气息-遗留的水晶碎片-远古王国的遗物-古城遗物-蘑菇袍子-浮游石原石-天界珍珠-越桔-魔力之花-野草莓-朗姆酒-裂空镖-甜瓜-飞盘 2-神圣葡萄酒-撒勒的印章-克尔顿的印章-肉块-天空树果实-石头-圣杯-天才的地图碎片-柴火-远古骑士的盔甲-无尽的永恒-使徒的气息-坚硬的龟壳-遗留的水晶碎片-突变苎麻花叶-苎麻花叶-副船长的戒指-步枪零件-黑色龙舌兰酒-烤硬的黑面包-虚空魔石碎片-格林赛罗斯的果核-跃翔药剂-突变草莓-暗黑城特产干酪-卡勒特勋章-迷幻晶石-高级钻石硬币-钻石硬币-混沌魔石碎片-碳结晶体-数据芯片-神秘的胶囊碎片-阳光硬币-魔刹石-命运硬币-砂砾-军用回旋镖-飞镖-砂砾-精灵香精-鸡腿-黑曜石-血滴石-紫玛瑙-金刚石-海蓝宝石-光辉魔石碎片-光辉魔石-次元碎片-卡勒特指令书-GBL教古书-凯丽的杂物-烤蘑菇-蘑菇酒-远古生命药剂-祝福之手-新手HP药剂-新手MP药剂-入门HP药剂-入门MP药剂-达人HP药剂-达人MP药剂-夜光石";
    vector<wstring> items;
    // 使用-分割文本
    split(filterItems, items, L"-");
    MapTraversalType data = GetMapData();
    vector<Position> result = vector<Position>();
    for (data.objTmp = 1; data.objTmp < data.objNum; data.objTmp++) {
        data.objPtr = GetTraversalPtr(data.start, data.objTmp, 2);
        data.objTypeA = mem.readInt(data.objPtr + 类型偏移);
        data.objCamp = mem.readInt(data.objPtr + 阵营偏移);
        if ((data.objTypeA == 289 || data.objTypeB == 289) && data.objCamp == 200) {
            auto itemNamePtr = mem.readLong(mem.readLong(data.objPtr + 地面物品) + 物品名称);
            auto itemNameBytes = mem.readBytes(itemNamePtr, 100);
            // 物品名称
            auto itemName = UnicodeToAnsi(itemNameBytes);
            // 物品名称在过滤列表中
            if (find(items.begin(), items.end(), itemName) != items.end()) {
                continue;
            }
            // 物体代码
            data.objCode = mem.readInt(data.objPtr + 代码偏移);
            // 物体血量
            data.objBlodd = mem.readInt(data.objPtr + 怪物血量);
            if (data.objCamp > 0 && data.objPtr != data.rwAddr) {
                    Position position = GetPosition(data.objPtr);
                    LOG("物体名称: 【%s】，类型：【%d】，坐标：【%d,%d】\n", WideStringToString(itemName).c_str(), data.objTypeA, position.x,
                        position.y);
                    result.insert(result.end(), position);
            }
        }
    }
    return result;
}

Position GetPosition(long long int ptr) {
    if (mem.readInt(ptr + 类型偏移) == 273) {
        auto data = mem.readLong(ptr + 读取坐标);
        return Position(mem.readFloat(data), mem.readFloat(data + 4), mem.readFloat(data + 8));
    } else {
        auto data = mem.readLong(ptr + 方向偏移);
        return Position(mem.readFloat(data + 32), mem.readFloat(data + +36), mem.readFloat(data + 40));
    }
}

Position GetNearestPosition(Position position, vector<Position> positions) {
    int minIndex = 0;
    int minRange = ComputedRange(position, positions[0]);;
    for (int i = 0; i < positions.size(); i++) {
        int range = ComputedRange(position, positions[i]);
        if (range < minRange) {
            minRange = range;
            minIndex = i;
        }
    }
    return positions[minIndex];
}

int ComputedRange(Position position, Position position1) {
    // 计算x坐标的差值
    int x = position.x - position1.x;
    // 计算y坐标的差值
    int y = position.y - position1.y;
    // 计算并返回两点之间的距离
    return (int) std::sqrt(x * x + y * y);
}


