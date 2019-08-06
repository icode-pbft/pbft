//
// Created by 18716 on 2019/8/1.
//

#ifndef PBFTV2_NEWMSG_H
#define PBFTV2_NEWMSG_H

#include <iostream>
#include <vector>
//#include <json/json.h>
using namespace std;

class newMsg {
private:
    string type;
    int serialNo;
    int viewNo;
    int nodeNo;
    int mainNo;
    vector<int> chooseNodes;
    string remark;
    string content;
    string result;
public:

//    Json::Value toJsonValue();

    newMsg();

    newMsg(const string &type);

    newMsg(const string &type, int serialNo, int viewNo, int nodeNo, int mainNo, const vector<int> &chooseNodes,
           string remark, const string &content, const string &result);

    const string &getType() const;

    void setType(const string &type);

    int getSerialNo() const;

    void setSerialNo(int serialNo);

    int getViewNo() const;

    void setViewNo(int viewNo);

    int getNodeNo() const;

    void setNodeNo(int nodeNo);

    int getMainNo() const;

    void setMainNo(int mainNo);

    const vector<int> &getChooseNodes() const;

    void setChooseNodes(const vector<int> &chooseNodes);

    const string &getContent() const;

    void setContent(const string &content);

    const string &getResult() const;

    void setResult(const string &result);

    const string &getRemark() const;

    void setRemark(const string &remark);
};


#endif //PBFTV2_NEWMSG_H
