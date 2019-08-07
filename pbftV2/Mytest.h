//
// Created by 18716 on 2019/8/1.
//

#ifndef PBFTV2_MYTEST_H
#define PBFTV2_MYTEST_H

#include <mutex>
#include <iostream>
#include <vector>
#include "msg/Msg.h"

using namespace std;

class Mytest {
private:
    static mutex mtx;
    static int i;

public:
    vector<Msg*> ve;
    static const mutex &getMtx();

    Mytest();

    ~Mytest();

    void test1(){
        for (Msg* msg:ve) {
            delete msg;
        }
    }


    static int getI();

    static void setI(int i);

    static void test(){
        mtx.lock();
        cout<<"*************"<<endl;
        mtx.unlock();
    }

};


#endif //PBFTV2_MYTEST_H
