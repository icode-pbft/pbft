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
    ve.push_back(new Msg());
    ve.push_back(new Msg());
    ve.push_back(new Msg());
    mtx.unlock();
}

Mytest::~Mytest() {
    cout<<"awsl\n";
    for (Msg* msg:ve) {
        delete msg;
    }
}
