//
// Created by 18716 on 2019/7/30.
//

#include "MyMutex.h"

int MyMutex::turn=0;
int MyMutex::flag[2];

int MyMutex::getTurn() {
    return turn;
}

void MyMutex::setTurn(int turn) {
    MyMutex::turn = turn;
}

int *MyMutex::getFlag() {
    return flag;
}
