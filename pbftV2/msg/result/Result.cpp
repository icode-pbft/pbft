//
// Created by 18716 on 2019/7/13.
//

#include "Result.h"

const string &Result::getType() const {
    return type;
}

void Result::setType(const string &type) {
    Result::type = type;
}

int Result::getSerialNo() const {
    return serialNo;
}

void Result::setSerialNo(int serialNo) {
    Result::serialNo = serialNo;
}

bool RightResult::isRight1() const {
    return isRight;
}

void RightResult::setIsRight(bool isRight) {
    RightResult::isRight = isRight;
}

RightResult::RightResult(bool isRight) : isRight(isRight) {
    this->setType("right");
}

RightResult::RightResult() {
    this->setType("right");
}

ErrorResult::ErrorResult(const bool out) : out(out) {
    this->setType("error");
}

ErrorResult::ErrorResult() {
    this->setType("error");
}
