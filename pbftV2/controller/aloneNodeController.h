//
// Created by 18716 on 2019/8/1.
//

#ifndef PBFTV2_ALONENODECONTROLLER_H
#define PBFTV2_ALONENODECONTROLLER_H

#include <vector>
#include "../msg/Msg.h"
#include "../config/Config.h"
#include <mutex>
#include <map>
#include <algorithm>
#include <thread>
using namespace std;

class aloneNodeController {

private:
//    /**
//     * <视图号，是否为投票节点>
//     */
//    static map<int,bool> isVote;

    static Config configSettings;

    static int nodeNo;

    static vector<Msg*> requestReadyList;

    static mutex mtx;

    mutable	std::mutex	m;

    static vector<aloneNodeController*> nodes;

    ////指定当前节点最多有多少个处理节点
    static const int nodeSizeMax;

    ////通过request的content
    vector<string> requestList;
    ////是否通过confirm消息
    bool isConfirm= false;
    ////pp消息队列
    vector<int> ppMsgList;
    ////pp消息待处理队列
    vector<Msg*> ppMsgReadyList;
    ////记录对应序列号收到的p消息的个数
    int pTimes[6][2]={0};
    ////p消息队列
    vector<int> pMsgList;
    ////p消息待处理队列
    vector<Msg*> pMsgReadyList;
    ////记录对应序列号收到的c消息的个数
    int cTimes[6][2]={0};
    ////c消息队列
    vector<int> cMsgList;
    ////c消息待处理队列
    vector<Msg*> cMsgReadyList;
    ////是否为主节点
    bool isMain= false;
    ////是否为投票节点
    bool isVote= false;
    ///系统
    int viewNo;
    ///系统节点的标识
    int systemId;

    int highLine;

    int lowLine;

    int serialNo=1;

    ////轮次
    int times=0;

    ////投票节点的个数
    int chooseSize;

public:

    aloneNodeController();

    /**
    * 启动方法
    * @param msg
    */
    static void start(Msg* msg);

    /**
    * 入口方法
    * @param msg
    */
    void action(Msg* msg);

    /**
    * 删除nodes里的一半节点并delete
    */
    static void removeHalfNodes();

    /**
     * 处理confirm消息
     * @param confirmMain
     */
    void dealWithConfirm(Msg* confirmMain);

    /**
    * 处理request消息
    * @param confirmMain
    */
    void dealWithRequest(Msg *request);

    /**
     * 处理待处理request消息队列
     * @param remark
     */
    void dealWithReadyRequest();

    /**
     * 处理pp消息
     * @param ppMsg
     */
    void dealWithPpMsg(Msg* ppMsg);

    /**
     * 处理待处理PP消息队列
     * @param serialNo
     */
    void dealWithReadyPpMsg(int serialNo);

    void dealWithReadyPpMsg();

    /**
     * 检查ppMsg是否符合要求
     * @param ppMsg
     * @return
     */
    bool checkPpMsg(Msg* ppMsg);

    /**
     * 处理p消息
     * @param pMsg
     */
    void dealWithPMsg(Msg* pMsg);

    /**
     * 处理待处理P消息队列
     * @param serialNo
     */
    void dealWithReadyPMsg(int serialNo);

    /**
     * 处理待处理消息队列
     * @param msg
     * @param serialNo
     */
    void dealWithReadyMsg(vector<Msg*>& msgs,int serialNo);

    void dealWithReadyMsg(vector<Msg*>* msgs);

    /**
     * 检查pMsg是否符合要求
     * @param pMsg
     * @return
     */
    bool checkPMsg(Msg* pMsg);

    /**
     * 处理c消息
     * @param commit
     */
    void dealWithCMsg(Msg* commit);

    /**
     * 处理待处理C消息队列
     * @param serialNo
     */
    void dealWithReadyCMsg(int serialNo);

    /**
     * 检查cMsg是否符合要求
     * @param commit
     * @return
     */
    bool checkCMsg(Msg* commit);

    /**
     * 检查Msg是否符合要求
     * @param ppMsg
     * @return
     */
    bool check(Msg * ppMsg);

    /**
     * 广播msg消息
     * @param msg
     */
    void sendMsg(Msg* msg);

    /**
     * 发送reply给系统
     * @param reply
     * @return
     */
    static int sendReply(Msg* reply);

    /**
     * 生成hash值并转化为string类型
     * @param content
     * @return
     */
    static string getHashCode(string content);

    /**
     * 对应的向量中是否有该int类型
     * @param ve
     * @param num
     * @return true : 在 false: 不在
     */
    static bool somethingIn(vector<int> ve,int num);

    /**
     * 对应的向量中是否有该字符串
     * @param ve
     * @param str
     * @return true : 在 false: 不在
     */
    static bool somethingIn(vector<string> ve,string str);

    /**
     * 对应的向量中是否有该消息
     * @param ve
     * @param object
     * @return true : 在 false: 不在
     */
    static bool objectIn(vector<Msg*> ve,Msg* object);

    /**
     * 对应的map中是否有对应的视图号
     * @param mp
     * @param viewNo
     * @return true : 在 false: 不在
     */
    static bool somethingIn(map<int,bool>mp,int viewNo);

    /**
     * 查看msgs向量中有无对应的序列号的msg
     * @param msgs
     * @param serialNo
     * @return
     */
    static bool MsgIn(vector<Msg*> msgs,int serialNo);

    /**
     * 检查节点列表中对应视图号的节点的位置，不存在则返回-1
     * @param ve
     * @param viewNo
     * @return
     */
    static int nodeIn(vector<aloneNodeController*> ve,int viewNo);

    /**
     * 将msg存入对应类型的待处理队列
     * @param node
     * @param msg
     */
    static void pushReadMsg(aloneNodeController*node, Msg* msg);

    /**
     * 初始化
     * @param nodeNo
     */
    static void init(int nodeNo);


    static void clearPMsgReadyList(int serialNo);

    static void clearCMsgReadyList(int serialNo);

    int getViewNo() const;

    void setViewNo(int viewNo);

    int getTimes() const;

    void setTimes(int times);


    /**
     * 测试用
     * @param msg
     * @param nodeNo
     */
    static void getMsg(Msg* msg,int nodeNo);


    static void putString(string msg);

};


#endif //PBFTV2_ALONENODECONTROLLER_H
