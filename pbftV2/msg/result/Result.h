//
// Created by 18716 on 2019/7/13.
//

#ifndef UNTITLED_RESULT_H
#define UNTITLED_RESULT_H

#include "iostream"
using namespace std;

class Result {
private:
    //共识的序列号
    int serialNo;
public:
    int getSerialNo() const;

    void setSerialNo(int serialNo);

private:
    string type;
public:
    const string &getType() const;

    void setType(const string &type);
};



class RightResult:public Result{
private:
    RightResult(bool isRight);

public:
    RightResult();

    bool isRight1() const;

    void setIsRight(bool isRight);

    bool isRight;
};

class ErrorResult:public Result{
public:
    const bool out=false;

    ErrorResult();

    ErrorResult(const bool out);
};

#endif //UNTITLED_RESULT_H
