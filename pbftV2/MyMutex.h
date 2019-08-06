//
// Created by 18716 on 2019/7/30.
//

#ifndef PBFTV2_MYMUTEX_H
#define PBFTV2_MYMUTEX_H


class MyMutex {
private:
    static int turn;
    static int flag[2];
public:
    static int getTurn();

    static void setTurn(int turn);

    static int *getFlag();


};


#endif //PBFTV2_MYMUTEX_H
