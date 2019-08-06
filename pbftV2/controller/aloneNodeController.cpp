//
// Created by 18716 on 2019/8/1.
//

#include "aloneNodeController.h"

//map<int,bool> aloneNodeController::isVote;
Config aloneNodeController::configSettings;
int aloneNodeController::nodeNo=1;
vector<aloneNodeController*> aloneNodeController::nodes;
vector<Msg*> aloneNodeController::requestReadyList;
mutex aloneNodeController::mtx;
const int aloneNodeController::nodeSizeMax=100;

/**
 * 启动方法
 * @param msg
 */
void aloneNodeController::start(Msg *msg) {


    ////lock
    mtx.lock();
    int index=nodeIn(nodes,msg->getViewNo());
    putString("主机节点个数"+to_string(nodes.size())+"----index=="+to_string(index)+"。。。。主机收到了数据的类型"+msg->getType());
    if(-1==index){
        putString("。。。。本主机不存在对应视图号"+to_string(msg->getViewNo())+"的node");
        if(nodes.size()>=nodeSizeMax){
            putString("。。。。本主机的节点数过多，清除一半的节点");
            removeHalfNodes();
        }
        aloneNodeController* node=new aloneNodeController();
        node->setViewNo(msg->getViewNo());
        nodes.push_back(node);
        putString("本主机当前节点数:"+to_string(nodes.size()));

        node->action(msg);
        ////unlock
        mtx.unlock();

        return;
    }


    nodes[index]->action(msg);
    ////unlock
    mtx.unlock();
}

/**
 * 入口方法
 * @param msg
 */
void aloneNodeController::action(Msg *msg) {
    string msgType=msg->getType();
    if("confirm"==msgType){
        dealWithConfirm(msg);
    } else if (!isVote&&!isConfirm){
        pushReadMsg(this,msg);
    } else if("request"==msgType){
        dealWithRequest(msg);
    } else if ("ppMsg"==msgType){
        dealWithPpMsg(msg);
    } else if("pMsg"==msgType){
        dealWithPMsg(msg);
    } else if("commit"==msgType){
        dealWithCMsg(msg);
    }
}

/**
 * 初始化静态数据
 * @param nodeNo
 */
void aloneNodeController::init(int nodeNo) {
    aloneNodeController::nodeNo=nodeNo;
    const char ConfigFile[]= "../config/config.txt";
    Config config(ConfigFile);
    configSettings=config;

}

/**
 * 生成hash值并转化为string类型
 * @param content
 * @return
 */
string aloneNodeController::getHashCode(string content) {
    hash<string> hashTest;
    return to_string(hashTest(content));
}

/**
 * 处理confirm消息
 * @param confirmMain
 */
void aloneNodeController::dealWithConfirm(Msg* confirmMain) {

    vector<int> chooseNodes=confirmMain->getChooseNodes();

    highLine=configSettings.Read("highLine",0);
    putString("highLine为:"+to_string(highLine));
    lowLine=configSettings.Read("lowLine",0);
    putString("lowLine为:"+to_string(lowLine));
    chooseSize=confirmMain->getChooseNodes().size();
    systemId = confirmMain->getSystemId();

    for (int i = 0; i < confirmMain->getChooseNodes().size(); ++i) {
        putString("投票节点有:"+to_string(confirmMain->getChooseNodes().at(i)));
    }
    cout<<endl;
    ///查看该节点是否是投票节点
    if(somethingIn(chooseNodes, nodeNo)){
        putString(to_string(nodeNo)+"号节点是投票节点");
        isVote= true;
        putString("主节点编号："+to_string(confirmMain->getMainNode())+"本主机编号为"+to_string(nodeNo));
        if(nodeNo==confirmMain->getMainNode()){
            putString("该节点是主节点");
            isMain=true;
            dealWithReadyRequest();
        } else{
            isMain=false;
            ////处理待处理的pp消息(避免之前有还未确认身份，就已收到pp消息的情况)
            dealWithReadyPpMsg();
        }
    }else{
        putString(to_string(nodeNo)+"号节点不是投票节点.");
        isVote= false;
        return;
    }

    isConfirm = true;

}

/**
 * 处理request消息
 * @param confirmMain
 */
void aloneNodeController::dealWithRequest(Msg *request) {

    if(!isMain){
        putString("非主节点，退出");
        return;
    }
    if(somethingIn(requestList,request->getContent())){
        ////已经处理过相同内容的request
        putString("已经处理过相同内容的request");
        return;
    }
    requestList.push_back(request->getContent());
    putString("处理request消息");
    ////初始化pTims和cTimes；
    putString("初始化pTims和cTimes");
    pTimes[times][0]=serialNo;
    cTimes[times][0]=serialNo;
    Msg* ppMsg=new Msg();
    ppMsg->setType("ppMsg");
    ppMsg->setViewNo(viewNo);
    ppMsg->setContent(request->getContent());
    ppMsg->setSerialNo(serialNo);

    ppMsgList.push_back(serialNo);

    ////使序列号加一
    serialNo+=1;
    times++;

    ppMsg->setRemark(request->getRemark());
    sendMsg(ppMsg);
    dealWithReadyPMsg(serialNo-1);

    Msg* pMsg=ppMsg;
    pMsg->setType("pMsg");
    sendMsg(pMsg);

}

/**
 * 处理pp消息
 * @param ppMsg
 */
void aloneNodeController::dealWithPpMsg(Msg* ppMsg) {
    if(!checkPpMsg(ppMsg)){
        putString("pp消息验证失败");
        return;
    } else{

        putString("pp消息通过");
        ////记录序列号
        int serialNo=ppMsg->getSerialNo();

        ////初始化pTims和cTimes；
        pTimes[times][0]=serialNo;
        cTimes[times][0]=serialNo;
        times++;

        ////将序列号放入已处理的pp消息列表里
        ppMsgList.push_back(ppMsg->getSerialNo());

        Msg* pMsg=ppMsg;
        pMsg->setType("pMsg");
        sendMsg(pMsg);

        ////处理对应序列号的待处理p消息
        dealWithReadyPMsg(ppMsg->getSerialNo());
    }
}

/**
 * 处理p消息
 * @param pMsg
 */
void aloneNodeController::dealWithPMsg(Msg* pMsg) {

    putString("开始处理p消息");
    if(!checkPMsg(pMsg)){
        putString("p消息验证失败");
        return;
    } else{
        putString("p消息第一步验证成功");

        int serialNo=pMsg->getSerialNo();

        ////当已处理的pp消息列表里没有对应的序列号时
        if(!somethingIn(ppMsgList,serialNo)){
            putString("因为没有收到的pp消息或者request消息，所以将p消息存入待处理列表,pMsgReadyList的大小"+to_string(pMsgReadyList.size()));
            pMsgReadyList.push_back(pMsg);
                return;
        }
        putString("p消息第二步验证成功");
        for (int i=0;i<times+1;i++){
            putString("pMsg::times="+to_string(times)+"循环到当前的序列号为"+to_string(pTimes[i][0])+"当前消息的序列号为"+to_string(serialNo));
            if (pTimes[i][0]==serialNo){

                int pMsgLength=++pTimes[i][1];
                putString("收到验证通过的p消息的个数"+to_string(pMsgLength));
                if(pMsgLength==chooseSize/3*2){
                    putString("收到了足够的p消息");
                    pMsgList.push_back(serialNo);

                    Msg* commit=pMsg;
                    commit->setType("commit");
                    sendMsg(commit);

                    ////处理待处理的commit消息
                    putString("开始处理c消息");
                    dealWithReadyCMsg(serialNo);
                }
                return;
            }
        }

    }
}

/**
 * 处理c消息
 * @param commit
 */
void aloneNodeController::dealWithCMsg(Msg* commit) {

    if(!checkCMsg(commit)){
        putString("c消息验证失败");
        return;
    } else{

        putString("c消息第一步验证成功");
        int serialNo=commit->getSerialNo();

        ////当该序列号对应的p消息没有通过时
        if(!somethingIn(pMsgList,commit->getSerialNo())){
            cMsgReadyList.push_back(commit);
            putString("因为没有收到足够的p消息，所以将c消息存入待处理列表,cMsgReadyList的大小"+to_string(cMsgReadyList.size()));
            return;
        }

        putString("c消息第二步验证成功");

        for (int i=0;i<times+1;i++){
            putString("commit::循环当前序列号为"+to_string(cTimes[i][0])+"当前消息的序列号为"+to_string(commit->getSerialNo()));
            if (cTimes[i][0]==serialNo){
                int cMsgLength=++cTimes[i][1];
                putString("收到验证通过的c消息的个数"+to_string(cMsgLength)+"    cMsgLength==chooseSize? :"+to_string(cMsgLength==chooseSize/3*2));
                if(cMsgLength==chooseSize/3*2){
                    cMsgList.push_back(commit->getSerialNo());
                    Msg*reply=commit;
                    reply->setType("reply");
                    reply->setResult(true);
                    reply->setNodeNo(nodeNo);
                    reply->setSystemId(systemId);
                    sendReply(reply);
                }
                return;
            }
        }

    }
}

/**
 * 处理待处理request消息队列
 * @param remark
 */
void aloneNodeController::dealWithReadyRequest() {
    cout<<"处理待处理的request消息\n";
    for (Msg* request:requestReadyList) {
        this->dealWithRequest(request);
    }
}

/**
 * 处理待处理PP消息队列
 * @param serialNo
 */
void aloneNodeController::dealWithReadyPpMsg(int serialNo) {

    putString("处理待处理的pp消息");
    dealWithReadyMsg(ppMsgReadyList,serialNo);
}

/**
 * 处理待处理PP消息队列
 */
void aloneNodeController::dealWithReadyPpMsg() {
    putString("处理待处理的pp消息");
    dealWithReadyMsg(ppMsgReadyList);
}

/**
 * 处理待处理P消息队列
 * @param serialNo
 */
void aloneNodeController::dealWithReadyPMsg(int serialNo) {
    putString("处理待处理的序列号为"+to_string(serialNo)+"p消息");
    dealWithReadyMsg(pMsgReadyList,serialNo);
}

/**
 * 处理待处理C消息队列
 * @param serialNo
 */
void aloneNodeController::dealWithReadyCMsg(int serialNo) {
    putString("处理待处理的c消息,总共"+to_string(cMsgReadyList.size())+"个c消息需要处理");
    dealWithReadyMsg(cMsgReadyList,serialNo);
}

/**
 * 处理待处理消息队列
 * @param msgs
 * @param serialNo
 */
void aloneNodeController::dealWithReadyMsg(vector<Msg*>& msgs,int serialNo) {
    putString("需要被处理的消息的个数为"+to_string(msgs.size())+"序列号为"+to_string(serialNo));

    for (int i = 0; i < msgs.size(); ++i) {
        if(msgs.at(i)->getSerialNo()==serialNo){
            action(msgs.at(i));
            msgs.erase(find(msgs.begin(),msgs.end(),msgs.at(i)));
            i--;
        }
    }
}

/**
 * 处理待处理消息队列
 * @param msgs
 */
void aloneNodeController::dealWithReadyMsg(vector<Msg*>& msgs) {
    putString("需要被处理的消息的个数为"+to_string(msgs.size()));

    for (int i = 0; i < msgs.size(); ++i) {
            action(msgs.at(i));
            msgs.erase(find(msgs.begin(),msgs.end(),msgs.at(i)));
            i--;
    }

}

/**
 * 对应的向量中是否有该字符串
 * @param ve
 * @param str
 * @return true : 在 false: 不在
 */
bool aloneNodeController::somethingIn(vector<string> ve, string str) {
    return std::find(ve.begin(),ve.end(),str)!=ve.end();
}

/**
 * 对应的向量中是否有该int类型
 * @param ve
 * @param num
 * @return true : 在 false: 不在
 */
bool aloneNodeController::somethingIn(vector<int> ve, int num) {
    return std::find(ve.begin(),ve.end(),num)!=ve.end();
}

/**
 * 对应的map中是否有对应的视图号
 * @param mp
 * @param viewNo
 * @return true : 在 false: 不在
 */
bool aloneNodeController::somethingIn(map<int, bool> mp, int viewNo) {
    return mp.find(viewNo)!=mp.end();
}

/**
 * 对应的向量中是否有该消息
 * @param ve
 * @param object
 * @return true : 在 false: 不在
 */
bool aloneNodeController::objectIn(vector<Msg *> ve, Msg *object) {
    return std::find(ve.begin(),ve.end(),object)!=ve.end();
}

/**
* @param ve
 * @param viewNo
 * @return -1 : 没有找到 other: 位置
 */
int aloneNodeController::nodeIn(vector<aloneNodeController *> ve,int viewNo) {

    for (int i = 0; i < ve.size(); ++i) {
        if(ve.at(i)->getViewNo()==viewNo){
            return i;
        }
    }

    return -1;
}

/**
 * 查看msgs向量中有无对应的序列号的msg
 * @param msgs
 * @param serialNo
 * @return
 */
bool aloneNodeController::MsgIn(vector<Msg *> msgs, int serialNo) {
    for (Msg* msg:msgs) {
        if(msg->getSerialNo()==serialNo){
            return true;
        }
    }
    return false;
}

/**
 * 广播msg消息
 * @param msg
 */
void aloneNodeController::sendMsg(Msg* msg) {
    string type=msg->getType();
    putString(type+"类型的数据开始广播了");
}

/**
 * 发送reply给系统
 * @param reply
 * @return
 */
int aloneNodeController::sendReply(Msg* reply) {
    putString("开始发送reply消息");
    return 0;
}

/**
 * 将msg存入对应类型的待处理队列
 * @param node
 * @param msg
 */
void aloneNodeController::pushReadMsg(aloneNodeController*node, Msg* msg) {


    string msgType = msg->getType();
    putString("因为节点还没有收到确认消息，无法判断自己能否处理，所以把"+msgType+"类型的消息放入待处理列表");
    if("request"==msgType){
        requestReadyList.push_back(msg);
        putString("request消息已放入待处理列表,当前大小"+to_string(requestReadyList.size()));
    }else if("ppMsg"==msgType){
        node->ppMsgReadyList.push_back(msg);
        putString("pp消息已放入待处理列表,当前大小"+to_string(node->ppMsgReadyList.size()));
    }else if("pMsg"==msgType){
        node->pMsgReadyList.push_back(msg);
        putString("p消息已放入待处理列表,当前大小"+to_string(node->pMsgReadyList.size()));
    }else if("commit"==msgType){
        node->cMsgReadyList.push_back(msg);
        putString("c消息已放入待处理列表,当前大小"+to_string(node->cMsgReadyList.size()));
    }

}

/**
 * 删除nodes里的一半节点并delete
 */
void aloneNodeController::removeHalfNodes() {
    int nodeSize=nodes.size();
    for (int i = 0; i < nodeSize/2; ++i) {
        delete nodes.at(i);
        nodes.erase(begin(nodes));
    }
}

/**
 * 检查ppMsg是否符合要求
 * @param ppMsg
 * @return
 */
bool aloneNodeController::checkPpMsg(Msg* ppMsg) {

    if (check(ppMsg)){
        ////检查是否收到重复pp消息
        for (int i = 0; i < ppMsgList.size(); ++i) {
            if (ppMsgList.at(i)==ppMsg->getSerialNo()){
                return false;
            }
        }
        return true;
    } else{
        return false;
    }
}

/**
 * 检查pMsg是否符合要求
 * @param pMsg
 * @return
 */
bool aloneNodeController::checkPMsg(Msg* pMsg) {
    return check(pMsg);
}

/**
 * 检查cMsg是否符合要求
 * @param commit
 * @return
 */
bool aloneNodeController::checkCMsg(Msg *commit) {
    return check(commit);
}

/**
 * 检查Msg是否符合要求
 * @param ppMsg
 * @return
 */
bool aloneNodeController::check(Msg *msg) {
    return msg->getRemark()==getHashCode(msg->getContent())&&msg->getViewNo()==viewNo
           &&msg->getSerialNo()>=lowLine&&msg->getSerialNo()<=highLine;
}


////getter和setter
int aloneNodeController::getTimes() const {
    return times;
}

void aloneNodeController::setTimes(int times) {
    aloneNodeController::times = times;
}

int aloneNodeController::getViewNo() const {
    return viewNo;
}

void aloneNodeController::setViewNo(int viewNo) {
    aloneNodeController::viewNo = viewNo;
}

/**
 * 测试用
 * @param msg
 * @param nodeNo
 */
void aloneNodeController::getMsg(Msg *msg,int nodeNo) {
    nodes.at(nodeNo)->action(msg);
}

/**
 * 无参构造方法
 */
aloneNodeController::aloneNodeController() {

}

void aloneNodeController::putString(string msg) {
    cout<<msg+"\n";
}

