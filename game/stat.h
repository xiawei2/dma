//
// Created by 17129 on 2024/6/14.
//

#include "Position.h"

#include "MapData.h"
#ifndef DMA_stat_H
#define DMA_stat_H

#endif //DMA_判断_H
#pragma once


int GetGameStat();

int GetPL();

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

Position GetPosition(long long ptr);

int GetRoleLevel();

INT GetGold();

INT GetItemNum(int ItemId);



std::wstring GetMapName();











