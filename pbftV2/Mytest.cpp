//
// Created by 18716 on 2019/8/1.
//

#include "Mytest.h"

mutex Mytest::mtx;
int Mytest::i=0;

const mutex &Mytest::getMtx() {
    return mtx;
}

int Mytest::getI() {
    return i;
}

void Mytest::setI(int i) {
    Mytest::i = i;
}

Mytest::Mytest() {
    mtx.lock();
    i++;
    cout<<"****"<<i<<endl;
    mtx.unlock();
}
