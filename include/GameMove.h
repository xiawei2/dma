//
// Created by 17129 on 2024/6/16.
//

#ifndef DMA_GAMEMOVE_H
#define DMA_GAMEMOVE_H
#pragma once
#include <time.h>
#include <valarray>
#include "keysym.h"
#include "qt5vncclient.h"
#include "utils.h"
#include "allHeader.h"


#endif //DMA_GAMEMOVE_H

class GameMove: public DNFCommon {
public:
    VncViewer vncViewer = VncViewer();
    double moveSpeed; // 移动速度

    long long int getTime() {
        // 获取当前时间点
        auto now = std::chrono::system_clock::now();

        // 转换为毫秒
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto value_ms = now_ms.time_since_epoch();
        long duration_ms = value_ms.count();
        return duration_ms;
    }

    // 上移动延迟
    long long int delayUp = getTime();
    // 下移动延迟
    long long int delayDown = getTime();
    // 左移动延迟
    long long int delayLeft = getTime();
    // 右移动延迟
    long long int delayRight = getTime();

    /**
     * 移动
     * @param roleX 角色x坐标
     * @param roleY 角色y坐标
     * @param moveX 目标点x坐标
     * @param moveY 目标点y坐标
     * @param moveType 移动方式 1-延迟移动 2-实时移动
     */
    void move(int roleX, int roleY, int moveX, int moveY, int moveType);
    void stopMove();
    void roleRun(bool run, int xDir);
    bool delayMove = false;
private:


};
inline GameMove gameMove;

