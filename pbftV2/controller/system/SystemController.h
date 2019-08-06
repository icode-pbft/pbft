/**
* @author 谭红霞
* @date 2019/8/2 14:14
*/
#ifndef PBFTV2_SYSTEMCONTROLLER_H
#define PBFTV2_SYSTEMCONTROLLER_H

#include "../aloneNodeController.h"
#include "../../msg/Msg.h"
#include <set>
#include <mutex>
using namespace std;
class SystemController {
private:
    /// 锁
    static mutex mtx;
    /// 用来测试的时候记录节点
    static vector<aloneNodeController*> nodes;
    /// 请求队列
    static vector<string> requestList;
    /// 记录选出的投票节点
    static vector<int> voteNodes;
    /// 记录视图号
    static int viewNo;
    /// 记录主节点
    static int mainNode;
    /// 记录轮次
    static int times;
    /// 记录序列号
    static int serialNo;
    /// 单例对象
    static SystemController* systemController;

    /// 视图编号--对象
    static map<int, SystemController*> systemControllers;
    /// 系统Id
    static int systemId;

    Config configSettings;
public:
    SystemController();
    ///共识开始
    static void startConsensus(Msg* request, vector<int> nodes);

    ///获取错误节点信息
    static void errorNodeMsg();

    void configTest();

    ////共识执行的函数
    static void send(Msg* request);

    ////视图变更
    void viewChange(vector<int> nodes);

    ////选出投票节点
    void chooseVoteNode(vector<int> nodes);

    ////选出主节点
    void chooseMainNode();

    ////获得hash值
    string getHashCode(string content);

    ////发送确认消息
    void sendConfirm(Msg confirmMain);

    ///接收Reply消息
    void getReply();

    static const mutex &getMtx();

    static const vector<aloneNodeController *> &getNodes();

    static void setNodes(const vector<aloneNodeController *> &nodes);

    static const vector<string> &getRequestList();

    static void setRequestList(const vector<string> &requestList);

    static const vector<int> &getVoteNodes();

    static void setVoteNodes(const vector<int> &voteNodes);

    static int getViewNo();

    static void setViewNo(int viewNo);

    static int getMainNode();

    static void setMainNode(int mainNode);

    static int getTimes();

    static void setTimes(int times);

    static int getSerialNo();

    static void setSerialNo(int serialNo);

    static SystemController *getSystemController();

    static void setSystemController(SystemController *systemController);

    const Config &getConfigSettings() const;

    void setConfigSettings(const Config &configSettings);

    const map<int, SystemController *> &getSystemControllers() const;

    void setSystemControllers(const map<int, SystemController *> &systemControllers);

    int getSystemId() const;

    void setSystemId(int systemId);
};


#endif //PBFTV2_SYSTEMCONTROLLER_H
