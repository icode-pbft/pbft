//
// Created by 18716 on 2019/8/1.
//

#include "newMsg.h"

const string &newMsg::getType() const {
    return type;
}

void newMsg::setType(const string &type) {
    newMsg::type = type;
}

int newMsg::getSerialNo() const {
    return serialNo;
}

void newMsg::setSerialNo(int serialNo) {
    newMsg::serialNo = serialNo;
}

int newMsg::getViewNo() const {
    return viewNo;
}

void newMsg::setViewNo(int viewNo) {
    newMsg::viewNo = viewNo;
}

int newMsg::getNodeNo() const {
    return nodeNo;
}

void newMsg::setNodeNo(int nodeNo) {
    newMsg::nodeNo = nodeNo;
}

int newMsg::getMainNo() const {
    return mainNo;
}

void newMsg::setMainNo(int mainNo) {
    newMsg::mainNo = mainNo;
}

const vector<int> &newMsg::getChooseNodes() const {
    return chooseNodes;
}

void newMsg::setChooseNodes(const vector<int> &chooseNodes) {
    newMsg::chooseNodes = chooseNodes;
}

const string &newMsg::getContent() const {
    return content;
}

void newMsg::setContent(const string &content) {
    newMsg::content = content;
}

const string &newMsg::getResult() const {
    return result;
}

void newMsg::setResult(const string &result) {
    newMsg::result = result;
}

newMsg::newMsg(const string &type, int serialNo, int viewNo, int nodeNo, int mainNo, const vector<int> &chooseNodes,
               string remark, const string &content, const string &result) : type(type), serialNo(serialNo),
                                                                             viewNo(viewNo), nodeNo(nodeNo),
                                                                             mainNo(mainNo), chooseNodes(chooseNodes),
                                                                             remark(remark), content(content),
                                                                             result(result) {}

newMsg::newMsg() {}

newMsg::newMsg(const string &type) : type(type) {}

//Json::Value newMsg::toJsonValue() {
//    Json::Value result;
//    Json::Value temp;
//
//    this->getType();
//    result["serialNo"]=this->getSerialNo();
//    result["viewNo"]=this->getViewNo();
//    result["nodeNo"]=this->getNodeNo();
//    result["mainNo"]=this->getMainNo();
//    result["chooseNodes"]=temp;
//    for (int i = 0; i < chooseNodes.size(); ++i) {
//        temp[i]=chooseNodes.at(i);
//    }
//    result["remark"]=this->getRemark();
//    result["content"]=this->getContent();
//    result["result"]=this->getResult();
//
//    return Json::Value();
//}

const string &newMsg::getRemark() const {
    return remark;
}

void newMsg::setRemark(const string &remark) {
    newMsg::remark = remark;
}
