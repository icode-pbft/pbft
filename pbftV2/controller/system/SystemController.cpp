/**
* @author ̷��ϼ
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
 * ���ڶ�ȡ�����ļ�
 * */
SystemController::SystemController() {
    const char ConfigFile[]= "../config/config.txt";
    Config config(ConfigFile);
    configSettings=config;
}

/**
 * ��ʶ��ʼ
 * */
void SystemController::startConsensus(Msg *request,vector<int> nodes) {
    if(times==0||times==6){
        SystemController *systemControllerTemp = new SystemController();
        systemControllers.insert(pair<int,SystemController*>(viewNo, systemControllerTemp));
    }

    //// ���ڱ���ͼ�µ�SystemController.
    SystemController* actionSystem = new SystemController();
    if(systemControllers[viewNo]!= nullptr){
        actionSystem = systemControllers[viewNo];
    }

    //// ��6�ι�ʶ֮����и�����ͼ.
    if (times==6) {
        actionSystem->viewChange(nodes);
        times = 0;
        serialNo++;
        //// ȷ����ݵ���Ϣ.

        Msg* confirm = new Msg();
        confirm->setType("confirm");
        confirm->setChooseNodes(voteNodes);
        confirm->setMainNode(mainNode);
        confirm->setViewNo(viewNo);

        //// ��֪���---�����߳�.
        actionSystem->sendConfirm(*confirm);
       // Sleep(1000);
    }
    times++;
    serialNo++;

    request->setSerialNo(serialNo);
    request->setRemark(actionSystem->getHashCode(request->getContent()));
    request->setSystemId(systemId);
    //// ����request����.
    actionSystem->send(request);
}

/**
 * ��ͼ���
 * */
void SystemController::viewChange(vector<int> nodes) {
    //// ѡȡͶƱ�ڵ�.
    chooseVoteNode(nodes);
    //// ѡȡ���ڵ�.
    chooseMainNode();
}

/**
 * ѡͶƱ�ڵ�
 * */
void SystemController::chooseVoteNode(vector<int> nodes) {
    //// ��ȡ�����ļ��е�ͶƱ�ڵ����.
    int voteSize=configSettings.Read("voteNodeSize",0);

    ////��������Ϊ��ǰʱ��.
    srand(unsigned(time(0)));
    for(int i = 0; i < voteSize; i++){
        while(1){//������ǲ�ͬ�������������ѭ��.
            int in =nodes[rand() % nodes.size()];
            if(voteNodes.empty()){
                voteNodes.push_back(in);
                break;
            }
            vector<int>::iterator it;
            it = find(voteNodes.begin(), voteNodes.end(), in);
            if(it == voteNodes.end()){//�����в����������.
                voteNodes.push_back(in);
                break;
            }
        }
    }
}

/**
 * ѡȡ���ڵ�
 * */
void SystemController::chooseMainNode() {
    int voteNodeSize = voteNodes.size();
    mainNode = viewNo % voteNodeSize;
}

/**
 * ����ȷ�������Ϣ
 * */
void SystemController::sendConfirm(Msg confirmMain) {
    ////�㲥�����нڵ�.
    ////{......}
//    thread confirmTread(  ,confirmMain);
//    confirmTread.join();
    //// ϵͳ����ȷ�������Ϣ֮����Ϣһ��֮���ٽ��к���Ĳ���.
    //Sleep(1000);
}

/**
 * ����request��Ϣ
 * */
void SystemController::send(Msg *request) {
    //// �㲥�����нڵ�(�������ڵ�����request���д���.
    //// {....}
//    thread sendTread(  ,request);
//    sendTread.join();
}

/**
 * ��ȡ��ϢժҪ
 * */
string SystemController::getHashCode(string content) {
    hash<string> hashTest;
    string hashCode = to_string(hashTest(content));
    return hashCode;
}

/**
 * ����ÿ���ڵ�Ļظ�
 * */
void SystemController::getReply() {
    ////һֱ�ڵȴ��Ž��սڵ㷢�����Ĺ�ʶ�����Ϣ.
    ////{.....}
}

/**
 * ��ȡ����ڵ����Ϣ������־�н��ж�ȡ��
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










