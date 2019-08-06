/**
* @author 谭红霞
* @date 2019/8/1 20:02
*/
#ifndef PBFTV2_NEWMSG_H
#define PBFTV2_NEWMSG_H

#include <vector>
#include "iostream"
#include <json/json.h>


using namespace std;

/**
 * @author lml
 * type enum:
 * request--request
 * confirm--confirm
 * pMsm--pMsg
 * ppMsg--ppMsg
 * commit--commit
 * reply--reply
 */
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
    bool result;
    int systemId;

public:

    Msg();

    Msg(const string &type, int viewNo, string remark, int serialNo,int nodeNo, const string &content);

    Msg(const string &type, int viewNo, string remark, int serialNo,int nodeNo);

    Msg(const string &type,  int serialNo, bool result);

    Msg(int viewNo, const vector<int> &chooseNodes, int mainNode, int systemId,const string &type);

    Msg(const string &type, int viewNo, const string &content,const string &remark);

    Msg(int viewNo, int serialNo, const string &content,const string &type,const string &remark);

    Msg(int viewNo, int serialNo, int nodeNo, bool result,const string &type);

    Json::Value toJsonValue();
    string toJsonStr();
    Msg* fromJson(const string& itemStr);

    static string getHashCode(string content);

    static Msg* createConfirm(int viewNo,int systemId,vector<int> chooseNodes,int mainNode);
    static Msg* createRequest(string content,int viewNo);
    static Msg* createPpMsg(string content,int serialNo,int viewNo);
    static Msg* createPMsg(string content,int serialNo,int viewNo);
    static Msg* createCMsg(string content,int serialNo,int viewNo);
    static Msg* createReply(bool result,int serialNo,int viewNo,int nodeNo);

    ////getter&setter


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

    const bool &getResult() const;

    void setResult(const bool &result);

    const string &getContent() const;

    void setContent(const string &content);

    const string &getRemark() const;

    void setRemark(const string &remark);

    int getSystemId() const;

    void setSystemId(int systemId);

};
#endif //PBFTV2_NEWMSG_H
