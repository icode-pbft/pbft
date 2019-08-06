/**
* @author 谭红霞
* @date 2019/8/1 20:02
*/
#ifndef PBFTV2_NEWMSG_H
#define PBFTV2_NEWMSG_H

#include <vector>
#include "iostream"
#include "result/Result.h"
#include "requestMsg/RequestMsg.h"
#include <json/json.h>

using namespace std;
class Msg {
private:
    string type;
    int viewNo;
    vector<int> chooseNodes;
    int mainNode;
    // size_t remark;
    string remark;
    int serialNo;
    int nodeNo;
    string content;
    string result;
    int systemIp; // 系统

public:
    Msg();
    Msg(const string &type, int viewNo, string remark, int serialNo,int nodeNo, const string &content);
    Msg(const string &type, int viewNo, string remark, int serialNo,int nodeNo);
    Msg(const string &type,  int serialNo, string result);
    Json::Value toJsonValue();
    string toJsonStr();
    static Msg* fromJson(const string& itemStr);

    const string &getType() const;

    void setType(const string &type);

    int getViewNo() const;

    void setViewNo(int viewNo);

    const vector<int> &getChooseNodes() const;

    void setChooseNodes(const vector<int> &chooseNodes);

    int getMainNode() const;

    void setMainNode(int mainNode);

    int getSerialNo() const;

    void setSerialNo(int serialNo);

    int getNodeNo() const;

    void setNodeNo(int nodeNo);

    const string &getResult() const;

    void setResult(const string &result);

    const string &getContent() const;

    void setContent(const string &content);

    const string &getRemark() const;

    void setRemark(const string &remark);

    int getSystemIp() const;

    void setSystemIp(int systemIp);

};
#endif //PBFTV2_NEWMSG_H
