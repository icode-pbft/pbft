/**
* @author 谭红霞
* @date 2019/8/2 14:14
*/
//#include <synchapi.h>
#include <pthread.h>
#include "SystemController.h"
#include "../../log/logger.h"

mutex SystemController::mtx;
vector<aloneNodeController*> SystemController::nodes;
vector<string> SystemController::requestList;
vector<int> SystemController::voteNodes;
int SystemController::times=0;
int SystemController::mainNode=0;
int SystemController::viewNo=0;
int SystemController::serialNo=0;
int SystemController::systemId=0;
SystemController *SystemController::systemController = new SystemController();
map<int, SystemController*> SystemController::systemControllers;

/**
 * 用于读取配置文件
 * */
SystemController::SystemController() {
    const char ConfigFile[]= "../config/config.txt";
    Config config(ConfigFile);
    configSettings=config;
}

/**
 * 共识开始
 * */
void SystemController::startConsensus(Msg *request,vector<int> nodes) {
    if(times==0||times==6){
        SystemController *systemControllerTemp = new SystemController();
        systemControllers.insert(pair<int,SystemController*>(viewNo, systemControllerTemp));
    }

    //// 用于本视图下的SystemController.
    SystemController* actionSystem = new SystemController();
    if(systemControllers[viewNo]!= nullptr){
        actionSystem = systemControllers[viewNo];
    }

    //// 若6次共识之后进行更换视图.
    if (times==6) {
        actionSystem->viewChange(nodes);
        times = 0;
        serialNo++;
        //// 确认身份的消息.

        Msg* confirm = new Msg();
        confirm->setType("confirm");
        confirm->setChooseNodes(voteNodes);
        confirm->setMainNode(mainNode);
        confirm->setViewNo(viewNo);

        //// 告知身份---创建线程.
        actionSystem->sendConfirm(*confirm);
       // Sleep(1000);
    }
    times++;
    serialNo++;

    request->setSerialNo(serialNo);
    request->setRemark(actionSystem->getHashCode(request->getContent()));
    request->setSystemId(systemId);
    //// 发送request请求.
    actionSystem->send(request);
}

/**
 * 视图变更
 * */
void SystemController::viewChange(vector<int> nodes) {
    //// 选取投票节点.
    chooseVoteNode(nodes);
    //// 选取主节点.
    chooseMainNode();
}

/**
 * 选投票节点
 * */
void SystemController::chooseVoteNode(vector<int> nodes) {
    //// 读取配置文件中的投票节点个数.
    int voteSize=configSettings.Read("voteNodeSize",0);

    ////设置种子为当前时间.
    srand(unsigned(time(0)));
    for(int i = 0; i < voteSize; i++){
        while(1){//插入的是不同的随机数才跳出循环.
            int in =nodes[rand() % nodes.size()];
            if(voteNodes.empty()){
                voteNodes.push_back(in);
                break;
            }
            vector<int>::iterator it;
            it = find(voteNodes.begin(), voteNodes.end(), in);
            if(it == voteNodes.end()){//数组中不存在这个数.
                voteNodes.push_back(in);
                break;
            }
        }
    }
}

/**
 * 选取主节点
 * */
void SystemController::chooseMainNode() {
    int voteNodeSize = voteNodes.size();
    mainNode = viewNo % voteNodeSize;
}

/**
 * 发送确认身份消息
 * */
void SystemController::sendConfirm(Msg confirmMain) {
    ////广播给所有节点.
    ////{......}
//    thread confirmTread(  ,confirmMain);
//    confirmTread.join();
    //// 系统发送确认身份消息之后，休息一秒之后再进行后面的操作.
    //Sleep(1000);
}

/**
 * 发送request消息
 * */
void SystemController::send(Msg *request) {
    //// 广播给所有节点(仅有主节点对这个request进行处理）.
    //// {....}
//    thread sendTread(  ,request);
//    sendTread.join();
}

/**
 * 获取消息摘要
 * */
string SystemController::getHashCode(string content) {
    hash<string> hashTest;
    string hashCode = to_string(hashTest(content));
    return hashCode;
}

/**
 * 接收每个节点的回复
 * */
void SystemController::getReply() {
    ////一直在等待着接收节点发过来的共识结果消息.
    ////{.....}
}

/**
 * 获取错误节点的消息（从日志中进行读取）
 * */
void SystemController::errorNodeMsg() {
    LOG(INFO);
}


const vector<aloneNodeController *> &SystemController::getNodes() {
    return nodes;
}

void SystemController::setNodes(const vector<aloneNodeController *> &nodes) {
    SystemController::nodes = nodes;
}

const vector<string> &SystemController::getRequestList() {
    return requestList;
}

void SystemController::setRequestList(const vector<string> &requestList) {
    SystemController::requestList = requestList;
}

const vector<int> &SystemController::getVoteNodes() {
    return voteNodes;
}

void SystemController::setVoteNodes(const vector<int> &voteNodes) {
    SystemController::voteNodes = voteNodes;
}

int SystemController::getViewNo() {
    return viewNo;
}

void SystemController::setViewNo(int viewNo) {
    SystemController::viewNo = viewNo;
}

int SystemController::getMainNode() {
    return mainNode;
}

void SystemController::setMainNode(int mainNode) {
    SystemController::mainNode = mainNode;
}

int SystemController::getTimes() {
    return times;
}

void SystemController::setTimes(int times) {
    SystemController::times = times;
}

int SystemController::getSerialNo() {
    return serialNo;
}

void SystemController::setSerialNo(int serialNo) {
    SystemController::serialNo = serialNo;
}

SystemController *SystemController::getSystemController() {
    return systemController;
}

void SystemController::setSystemController(SystemController *systemController) {
    SystemController::systemController = systemController;
}

const Config &SystemController::getConfigSettings() const {
    return configSettings;
}

void SystemController::setConfigSettings(const Config &configSettings) {
    SystemController::configSettings = configSettings;
}

const mutex &SystemController::getMtx() {
    return mtx;
}

const map<int, SystemController *> &SystemController::getSystemControllers() const {
    return systemControllers;
}

void SystemController::setSystemControllers(const map<int, SystemController *> &systemControllers) {
    SystemController::systemControllers = systemControllers;
}

int SystemController::getSystemId() const {
    return systemId;
}

void SystemController::setSystemId(int systemId) {
    SystemController::systemId = systemId;
}

void SystemController::configTest() {
    cout<<configSettings.Read("port",0);
}










