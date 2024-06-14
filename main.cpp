//
// Created by 17129 on 2024/6/14.
//

#include "main.h"
#include "game/stat.h"
#include <iostream>
#include <cstdint>
int main(){
    auto wmapName = GetMapName();
    std::string mapName = WideStringToString(wmapName);
    std::cout << mapName<<std::endl;
    std::wcout<< wmapName;
    return 0;
}

