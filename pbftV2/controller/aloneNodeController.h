//
// Created by 18716 on 2019/8/1.
//

#ifndef PBFTV2_ALONENODECONTROLLER_H
#define PBFTV2_ALONENODECONTROLLER_H

#include <vector>
#include <mutex>
#include <algorithm>
#include <thread>

#include "../msg/Msg.h"
#include "../config/Config.h"

using namespace std;
class aloneNodeController {

private:
    /// request消息待处理队列
    static vector<Msg*> requestReadyList;
    static vector<aloneNodeController*> nodes;
    /// nodes中最多存放节点的个数
    static const int nodeSizeMax;
    /// 节点编号
    static int nodeNo;
    /// 通过的pp消息
    vector<Msg*> ppMsgs;
    /// pp消息队列
    vector<int> ppMsgList;
    /// pp消息待处理队列
    vector<Msg*> ppMsgReadyList;
    /// 记录对应序列号收到的p消息的个数
    int pTimes[6][2];
    /// 通过的p消息队列
    vector<int> pMsgList;
    /// p消息待处理队列
    vector<Msg*> pMsgReadyList;
    /// 记录对应序列号收到的c消息的个数
    int cTimes[6][2];
    /// 通过的c消息队列
    vector<int> cMsgList;
    /// c消息待处理队列
    vector<Msg*> cMsgReadyList;
    /// 是否为投票节点
    bool isVote;
    /// 视图编号
    int viewNo;
    /// 高水位线
    int highLine;
    /// 低水位线
    int lowLine;
    /// 序列号
    int serialNo;
    /// 轮次
    int times;
    /// 投票节点的个数
    int chooseSize;
    /// 用于读取配置文件
    Config configSettings;
    /// 系统ip
    int systemIp;
public:
public:
    /**
     * 共识开始
     * @param msg
     */
    static void start(Msg* msg);

    /**
     * 删除nodes里的一半节点并delete
     */
    static void removeHalfNodes();

    /**
     * 收到消息以及对应处理
     * @param msg
     */
    void action(Msg* msg);

    /**
     * 处理Confirm消息
     * @param confirmMain
     */
    void dealWithConfirm(Msg* confirmMain);

    /**
     * 处理Request消息
     * @param request
     */
    void dealWithRequest(Msg* request);

    /**
     * 处理pp消息
     * @param ppMsg
     */
    void dealWithPpMsg(Msg* ppMsg);

    /**
     * 处理p消息
     * @param pMsg
     */
    void dealWithPMsg(Msg* pMsg);

    /**
     * 处理c消息
     * @param commit
     */
    void dealWithCMsg(Msg* commit);

    /**
     * pp阶段检查收到的消息其序列号以及视图号是否符合标准
     * @param ppMsg
     * @return 符合为true
     */
    bool checkPpMsg(Msg* ppMsg);

    /**
     * p阶段消息检查收到的消息其摘要、视图号、序列号是否符合标准
     * @param pMsg
     * @return
     */
    bool checkPMsg(Msg* pMsg);


    /**
     * C阶段消息检查收到的消息其摘要、视图号、序列号是否符合标准
     * @param commit
     * @return
     */
    bool checkCMsg(Msg* commit);

    /**
     * 检查消息
     * @param ppMsg
     * @return
     */
    bool check(Msg * ppMsg);

    /**
     * 节点发消息
     * @param msg
     */
    static void sendMsg(Msg* msg);

    /**
     * 节点向系统发送reply消息
     * @param reply
     */
    static void sendReply(Msg* reply);

    /**
     * 获得字符串的hash值
     * @param content
     * @return 哈希值的十进制无符号值
     */
    static size_t getHashCode(string content);

    /**
     * 读取配置文件初始化参数
     */
    void initConfig();

    /**
     * 对应的向量中是否有该int类型
     * @param ve
     * @param num
     * @return
     */
    static bool somethingIn(vector<int> ve,int num);

    /**
     * 检查节点列表中对应视图号的节点的位置，不存在则返回-1
     * @param ve
     * @param viewNo
     * @return
     */
    static int nodeIn(vector<aloneNodeController*> ve,int viewNo);

    /**
     * 写日志
     * @param message
     */
    static void writeLog(string message);

    /// getter和setter方法
    static void setNodeNo(int nodeNo);

    int getViewNo() const;

    void setViewNo(int viewNo);

    int getSystemIp() const;

};


#endif //PBFTV2_ALONENODECONTROLLER_H
