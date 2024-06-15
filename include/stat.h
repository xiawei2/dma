//
// Created by 17129 on 2024/6/14.
//

#include "Position.h"

#include "MapData.h"
#ifndef DMA_stat_H
#define DMA_stat_H

#endif //DMA_判断_H
#pragma once

/**
 * 游戏状态
 *
 * @return int 0选角 1城镇 2选图 3图内 5选择频道
 */
int GetGameStat();

int GetPL();
/**
  * 是否城镇
  *
  * @return boolean
  */
bool IsTown();

bool IsOpenDoor();

bool IsBossRoom();

bool FtilterItem(std::string ItemName);

bool ContainsItem();

bool ArePointsEqual(Position index1, Position index2);

bool ContainsMonster();

int64_t GetPersonPrt();

int GetLevel();

bool IsPass();


int GetRoleLevel();

INT GetGold();

INT GetItemNum(int ItemId);

std::wstring GetMapName();
/**
 * 获取名望
 * @return
 */
INT GetFame();
/**
 * 获取背包负重
 */
 INT GetBagWeight();
//0站立、6跳跃、8普攻、14跑动
INT GetPersonState();











