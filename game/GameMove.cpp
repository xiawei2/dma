//
// Created by 17129 on 2024/6/16.
//
#include "GameMove.h"

void GameMove::move(int roleX, int roleY, int moveX, int moveY, int moveType) {
    int yDelay; // 移动时间x
    int xDelay; // 移动时间y
    int xDir;
    int yDir;

    // 是否需要跑动
    bool needRun = false;
    // 取动方向
    xDir = (roleX - moveX) > 0 ? XK_Left : XK_Right;
    yDir = (roleY - moveY) > 0 ? XK_Up : XK_Down;
    // 获取移动时间
    xDelay = abs(roleX - moveX) * moveSpeed<100?100:abs(roleX - moveX) * moveSpeed;
    yDelay = abs(roleY - moveY) * moveSpeed<100?100:abs(roleY - moveY) * moveSpeed;
    // x移动延迟大于200，则需要跑动
    if (xDelay > 200) {
        needRun = true;
        // 跑动时，x方向移动时间减半
        xDelay = xDelay / 2;
    }
    auto now = GetTime();
    if (xDir == XK_Left) {
        // 判断内存中的左按键延迟是否大于当前系统时间，如果大于则加上当前传入的延迟
        if (delayLeft > now) {
            vncViewer.KeyPressUp(XK_Right);
            vncViewer.KeyPressDown(XK_Left);
            if (GetPersonState() != 14) {
                roleRun(needRun, xDir);
            }
        } else {
            // 如果小于当前系统时间,则代表已经弹起，则需要重新调用跑动
            roleRun(needRun, xDir);
        }
        delayLeft = now + xDelay;
        delayRight = 0;
    }
    if (xDir == XK_Right) {
        // 判断内存中的左按键延迟是否大于当前系统时间，如果大于则加上当前传入的延迟
        if (delayRight > now) {
            vncViewer.KeyPressUp(XK_Left);
            vncViewer.KeyPressDown(XK_Right);
            if (GetPersonState() != 14) {
                roleRun(needRun, xDir);
            }
        } else {
            // 如果小于当前系统时间,则代表已经弹起，则需要重新调用跑动
            roleRun(needRun, xDir);
        }
        delayRight = now + xDelay;
        delayLeft = 0;
    }
    if (yDir == XK_Up) {
        // 判断内存中的左按键延迟是否大于当前系统时间，如果大于则加上当前传入的延迟
        if (delayUp > now) {
            vncViewer.KeyPressUp(XK_Down);
            vncViewer.KeyPressDown(XK_Up);
        } else {
//                vncViewer.KeyPressUp(yDir);
            vncViewer.KeyPressDown(yDir);
        }
        delayUp = now + yDelay;
        delayDown = 0;

    }
    if (yDir == XK_Down) {
        // 判断内存中的左按键延迟是否大于当前系统时间，如果大于则加上当前传入的延迟
        if (delayDown > now) {
            vncViewer.KeyPressUp(XK_Up);
            vncViewer.KeyPressDown(XK_Down);
        } else {
//                vncViewer.KeyPressUp(yDir);
            vncViewer.KeyPressDown(yDir);
        }
        delayDown = now + yDelay;
        delayUp = 0;

    }


    if (!delayMove) {
        delayMove = true;
        std::thread *m_runThread = new std::thread([this]() {
            bool left = false;
            bool right = false;
            bool up = false;
            bool down = false;
            while (delayMove) {
                auto nowTime = GetTime();
                // 当按键延迟小于当前系统时间,弹起该按键，并置该按键为false状态，当全部按键为false时退出循环，进程结束
                if (delayLeft < nowTime) {

                    vncViewer.KeyPressUp(XK_Left);

                    left = true;
                } else {
                    left = false;
                }
                if (delayRight < nowTime) {

                    vncViewer.KeyPressUp(XK_Right);

                    right = true;
                } else {
                    right = false;
                }
                if (delayUp < nowTime) {

                    vncViewer.KeyPressUp(XK_Up);

                    up = true;
                } else {
                    up = false;
                }
                if (delayDown < nowTime) {

                    vncViewer.KeyPressUp(XK_Down);

                    down = true;
                } else {
                    down = false;
                }
                if (up && down && left && right) {
                    vncViewer.KeyPressUp(XK_Up);

                    vncViewer.KeyPressUp(XK_Right);
                    vncViewer.KeyPressUp(XK_Left);
                    vncViewer.KeyPressUp(XK_Down);
                    delayMove = false;
                }
            };
            stopMove();
        });
        m_runThread->detach();
    }
    if (moveType == 1) {
        Sleep(xDelay > yDelay ? xDelay : yDelay);
    }
}

void GameMove::roleRun(bool run, int xDir) {
    if (run) {
        if (GetPersonState() == 14) {
            return;
        }
        vncViewer.KeyPressUp(xDir);
        // 连续按2次跑动
        vncViewer.KeyPress(xDir, rnd(40, 80));
        Sleep(30);
        vncViewer.KeyPressDown(xDir);
        Sleep(30);
    } else {
        // 不跑动，走
        vncViewer.KeyPressDown(xDir);
    }
}

void GameMove::stopMove() {
    delayMove = false;
    vncViewer.KeyPressUp(XK_Up);
    Sleep(30);
    vncViewer.KeyPressUp(XK_Right);
    Sleep(30);
    vncViewer.KeyPressUp(XK_Left);
    Sleep(30);
    vncViewer.KeyPressUp(XK_Down);
}
