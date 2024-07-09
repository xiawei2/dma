//
// Created by 17129 on 2024/6/15.
//

#include "Auto.h"


void GameAuto::Fight() {
    if (gameMap.GetGameStat() != 3&&!firstIntoMap&&!gameMap.IsPass()) {
        return;
    }
    // 首次进图操作
    firstIntoMap = false;
    gameMap.path.clear();
    while (!gameMap.IsPass()&&stat) {
        if (gameMap.IsOpenDoor()) {
            gameMap.passMap();
        } else {
            gameMap.AttactMonster();
        }
        Sleep(100);
    }
}

void GameAuto::Finish() {
    if (gameMap.GetGameStat() != 3) {
        return;
    }

    if (stat&&gameMap.IsPass()&&!firstIntoMap){
        times++;
        printf("已通关:%d次！\n",times);
    }
    gameMove.vncViewer.KeyPress(XK_V, rnd(40, 120));
    while (true&&stat){
        if (gameMap.取翻牌状态()==4||gameMap.取翻牌状态()==0){
            Sleep(1000);
            gameMove.vncViewer.KeyPress(XK_1, rnd(40, 120));

        }
        if(gameMap.取翻牌状态()==1){
            break;
        }

    }
    if (gameMap.GetBagWeight()>50){
        printf("出售物品\n");
        Sleep(500);
        gameMove.vncViewer.KeyPress(XK_A, rnd(40, 120));
        Sleep(500);
        gameMove.vncViewer.KeyPress(XK_space, rnd(40, 120));
        Sleep(500);
        gameMove.vncViewer.KeyPress(XK_Left, rnd(40, 120));
        Sleep(500);
        gameMove.vncViewer.KeyPress(XK_space, rnd(40, 120));
    }
    if (gameMap.GetPL() > 0) {
        while (gameMap.IsPass()&&stat) {
            gameMap.PickItem();
            if (gameMap.IsJiaBaiLi()){
                gameMove.vncViewer.KeyPress(XK_Escape, rnd(40, 120));
            }
            gameMove.vncViewer.KeyPress(XK_F10, rnd(40, 120));
            Sleep(1000);
        }
        Sleep(1000);
        gameMove.vncViewer.KeyPress(XK_Control_L, rnd(40, 120));

    }else{
        stat = false;
    }
}

void GameAuto::Start() {

    mem.GetKeyboard()->UpdateKeys();
    std::thread *t = new std::thread([this]() {
        while (true) {
            // end
            if(mem.GetKeyboard()->IsKeyDown(35)){
                stat = false;
            }
            // home
            if (mem.GetKeyboard()->IsKeyDown(36)){
                gameMap.speed=0;
                stat = true;
            }
            if (mem.GetKeyboard()->IsKeyDown(112)){ //F1
                gameMap.武器冰冻(999999999);
            }
            if (mem.GetKeyboard()->IsKeyDown(113)){ //F2
                gameMap.无视建筑();
            }
            Sleep(500);
        };
    });
    t->detach();
    while (true){
        if (stat){
            Fight();
            Finish();
        }
        Sleep(500);
    }

}
