//
// Created by 17129 on 2024/6/14.
//

#include "Position.h"


int decode(int64_t address) {
    return mem.Read<int>(address);
}

Position::Position(int i, int i1, int i2 ) {
    x=i;
    y=i1;
    z=i2;
}
