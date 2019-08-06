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
 * ��������
 * @param msg
 */
void aloneNodeController::start(Msg *msg) {


    ////lock
    mtx.lock();
    int index=nodeIn(nodes,msg->getViewNo());
    putString("�����ڵ����"+to_string(nodes.size())+"----index=="+to_string(index)+"�������������յ������ݵ�����"+msg->getType());
    if(-1==index){
        putString("�������������������ڶ�Ӧ��ͼ��"+to_string(msg->getViewNo())+"��node");
        if(nodes.size()>=nodeSizeMax){
            putString("���������������Ľڵ������࣬���һ��Ľڵ�");
            removeHalfNodes();
        }
        aloneNodeController* node=new aloneNodeController();
        node->setViewNo(msg->getViewNo());
        nodes.push_back(node);
        putString("��������ǰ�ڵ���:"+to_string(nodes.size()));

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
 * ��ڷ���
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
 * ��ʼ����̬����
 * @param nodeNo
 */
void aloneNodeController::init(int nodeNo) {
    aloneNodeController::nodeNo=nodeNo;
    const char ConfigFile[]= "../config/config.txt";
    Config config(ConfigFile);
    configSettings=config;

}

/**
 * ����hashֵ��ת��Ϊstring����
 * @param content
 * @return
 */
string aloneNodeController::getHashCode(string content) {
    hash<string> hashTest;
    return to_string(hashTest(content));
}

/**
 * ����confirm��Ϣ
 * @param confirmMain
 */
void aloneNodeController::dealWithConfirm(Msg* confirmMain) {

    vector<int> chooseNodes=confirmMain->getChooseNodes();

    highLine=configSettings.Read("highLine",0);
    putString("highLineΪ:"+to_string(highLine));
    lowLine=configSettings.Read("lowLine",0);
    putString("lowLineΪ:"+to_string(lowLine));
    chooseSize=confirmMain->getChooseNodes().size();
    systemId = confirmMain->getSystemId();

    for (int i = 0; i < confirmMain->getChooseNodes().size(); ++i) {
        putString("ͶƱ�ڵ���:"+to_string(confirmMain->getChooseNodes().at(i)));
    }
    cout<<endl;
    ///�鿴�ýڵ��Ƿ���ͶƱ�ڵ�
    if(somethingIn(chooseNodes, nodeNo)){
        putString(to_string(nodeNo)+"�Žڵ���ͶƱ�ڵ�");
        isVote= true;
        putString("���ڵ��ţ�"+to_string(confirmMain->getMainNode())+"���������Ϊ"+to_string(nodeNo));
        if(nodeNo==confirmMain->getMainNode()){
            putString("�ýڵ������ڵ�");
            isMain=true;
            dealWithReadyRequest();
        } else{
            isMain=false;
            ////����������pp��Ϣ(����֮ǰ�л�δȷ����ݣ������յ�pp��Ϣ�����)
            dealWithReadyPpMsg();
        }
    }else{
        putString(to_string(nodeNo)+"�Žڵ㲻��ͶƱ�ڵ�.");
        isVote= false;
        return;
    }

    isConfirm = true;

}

/**
 * ����request��Ϣ
 * @param confirmMain
 */
void aloneNodeController::dealWithRequest(Msg *request) {

    if(!isMain){
        putString("�����ڵ㣬�˳�");
        return;
    }
    if(somethingIn(requestList,request->getContent())){
        ////�Ѿ��������ͬ���ݵ�request
        putString("�Ѿ��������ͬ���ݵ�request");
        return;
    }
    requestList.push_back(request->getContent());
    putString("����request��Ϣ");
    ////��ʼ��pTims��cTimes��
    putString("��ʼ��pTims��cTimes");
    pTimes[times][0]=serialNo;
    cTimes[times][0]=serialNo;
    Msg* ppMsg=new Msg();
    ppMsg->setType("ppMsg");
    ppMsg->setViewNo(viewNo);
    ppMsg->setContent(request->getContent());
    ppMsg->setSerialNo(serialNo);

    ppMsgList.push_back(serialNo);

    ////ʹ���кż�һ
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
 * ����pp��Ϣ
 * @param ppMsg
 */
void aloneNodeController::dealWithPpMsg(Msg* ppMsg) {
    if(!checkPpMsg(ppMsg)){
        putString("pp��Ϣ��֤ʧ��");
        return;
    } else{

        putString("pp��Ϣͨ��");
        ////��¼���к�
        int serialNo=ppMsg->getSerialNo();

        ////��ʼ��pTims��cTimes��
        pTimes[times][0]=serialNo;
        cTimes[times][0]=serialNo;
        times++;

        ////�����кŷ����Ѵ����pp��Ϣ�б���
        ppMsgList.push_back(ppMsg->getSerialNo());

        Msg* pMsg=ppMsg;
        pMsg->setType("pMsg");
        sendMsg(pMsg);

        ////�����Ӧ���кŵĴ�����p��Ϣ
        dealWithReadyPMsg(ppMsg->getSerialNo());
    }
}

/**
 * ����p��Ϣ
 * @param pMsg
 */
void aloneNodeController::dealWithPMsg(Msg* pMsg) {

    putString("��ʼ����p��Ϣ");
    if(!checkPMsg(pMsg)){
        putString("p��Ϣ��֤ʧ��");
        return;
    } else{
        putString("p��Ϣ��һ����֤�ɹ�");

        int serialNo=pMsg->getSerialNo();

        ////���Ѵ����pp��Ϣ�б���û�ж�Ӧ�����к�ʱ
        if(!somethingIn(ppMsgList,serialNo)){
            putString("��Ϊû���յ���pp��Ϣ����request��Ϣ�����Խ�p��Ϣ����������б�,pMsgReadyList�Ĵ�С"+to_string(pMsgReadyList.size()));
            pMsgReadyList.push_back(pMsg);
                return;
        }
        putString("p��Ϣ�ڶ�����֤�ɹ�");
        for (int i=0;i<times+1;i++){
            putString("pMsg::times="+to_string(times)+"ѭ������ǰ�����к�Ϊ"+to_string(pTimes[i][0])+"��ǰ��Ϣ�����к�Ϊ"+to_string(serialNo));
            if (pTimes[i][0]==serialNo){

                int pMsgLength=++pTimes[i][1];
                putString("�յ���֤ͨ����p��Ϣ�ĸ���"+to_string(pMsgLength));
                if(pMsgLength==chooseSize/3*2){
                    putString("�յ����㹻��p��Ϣ");
                    pMsgList.push_back(serialNo);

                    Msg* commit=pMsg;
                    commit->setType("commit");
                    sendMsg(commit);

                    ////����������commit��Ϣ
                    putString("��ʼ����c��Ϣ");
                    dealWithReadyCMsg(serialNo);
                }
                return;
            }
        }

    }
}

/**
 * ����c��Ϣ
 * @param commit
 */
void aloneNodeController::dealWithCMsg(Msg* commit) {

    if(!checkCMsg(commit)){
        putString("c��Ϣ��֤ʧ��");
        return;
    } else{

        putString("c��Ϣ��һ����֤�ɹ�");
        int serialNo=commit->getSerialNo();

        ////�������кŶ�Ӧ��p��Ϣû��ͨ��ʱ
        if(!somethingIn(pMsgList,commit->getSerialNo())){
            cMsgReadyList.push_back(commit);
            putString("��Ϊû���յ��㹻��p��Ϣ�����Խ�c��Ϣ����������б�,cMsgReadyList�Ĵ�С"+to_string(cMsgReadyList.size()));
            return;
        }

        putString("c��Ϣ�ڶ�����֤�ɹ�");

        for (int i=0;i<times+1;i++){
            putString("commit::ѭ����ǰ���к�Ϊ"+to_string(cTimes[i][0])+"��ǰ��Ϣ�����к�Ϊ"+to_string(commit->getSerialNo()));
            if (cTimes[i][0]==serialNo){
                int cMsgLength=++cTimes[i][1];
                putString("�յ���֤ͨ����c��Ϣ�ĸ���"+to_string(cMsgLength)+"    cMsgLength==chooseSize? :"+to_string(cMsgLength==chooseSize/3*2));
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
 * ���������request��Ϣ����
 * @param remark
 */
void aloneNodeController::dealWithReadyRequest() {
    cout<<"����������request��Ϣ\n";
    for (Msg* request:requestReadyList) {
        this->dealWithRequest(request);
    }
}

/**
 * ���������PP��Ϣ����
 * @param serialNo
 */
void aloneNodeController::dealWithReadyPpMsg(int serialNo) {

    putString("����������pp��Ϣ");
    dealWithReadyMsg(ppMsgReadyList,serialNo);
}

/**
 * ���������PP��Ϣ����
 */
void aloneNodeController::dealWithReadyPpMsg() {
    putString("����������pp��Ϣ");
    dealWithReadyMsg(ppMsgReadyList);
}

/**
 * ���������P��Ϣ����
 * @param serialNo
 */
void aloneNodeController::dealWithReadyPMsg(int serialNo) {
    putString("�������������к�Ϊ"+to_string(serialNo)+"p��Ϣ");
    dealWithReadyMsg(pMsgReadyList,serialNo);
}

/**
 * ���������C��Ϣ����
 * @param serialNo
 */
void aloneNodeController::dealWithReadyCMsg(int serialNo) {
    putString("����������c��Ϣ,�ܹ�"+to_string(cMsgReadyList.size())+"��c��Ϣ��Ҫ����");
    dealWithReadyMsg(cMsgReadyList,serialNo);
}

/**
 * �����������Ϣ����
 * @param msgs
 * @param serialNo
 */
void aloneNodeController::dealWithReadyMsg(vector<Msg*>& msgs,int serialNo) {
    putString("��Ҫ���������Ϣ�ĸ���Ϊ"+to_string(msgs.size())+"���к�Ϊ"+to_string(serialNo));

    for (int i = 0; i < msgs.size(); ++i) {
        if(msgs.at(i)->getSerialNo()==serialNo){
            action(msgs.at(i));
            msgs.erase(find(msgs.begin(),msgs.end(),msgs.at(i)));
            i--;
        }
    }
}

/**
 * �����������Ϣ����
 * @param msgs
 */
void aloneNodeController::dealWithReadyMsg(vector<Msg*>& msgs) {
    putString("��Ҫ���������Ϣ�ĸ���Ϊ"+to_string(msgs.size()));

    for (int i = 0; i < msgs.size(); ++i) {
            action(msgs.at(i));
            msgs.erase(find(msgs.begin(),msgs.end(),msgs.at(i)));
            i--;
    }

}

/**
 * ��Ӧ���������Ƿ��и��ַ���
 * @param ve
 * @param str
 * @return true : �� false: ����
 */
bool aloneNodeController::somethingIn(vector<string> ve, string str) {
    return std::find(ve.begin(),ve.end(),str)!=ve.end();
}

/**
 * ��Ӧ���������Ƿ��и�int����
 * @param ve
 * @param num
 * @return true : �� false: ����
 */
bool aloneNodeController::somethingIn(vector<int> ve, int num) {
    return std::find(ve.begin(),ve.end(),num)!=ve.end();
}

/**
 * ��Ӧ��map���Ƿ��ж�Ӧ����ͼ��
 * @param mp
 * @param viewNo
 * @return true : �� false: ����
 */
bool aloneNodeController::somethingIn(map<int, bool> mp, int viewNo) {
    return mp.find(viewNo)!=mp.end();
}

/**
 * ��Ӧ���������Ƿ��и���Ϣ
 * @param ve
 * @param object
 * @return true : �� false: ����
 */
bool aloneNodeController::objectIn(vector<Msg *> ve, Msg *object) {
    return std::find(ve.begin(),ve.end(),object)!=ve.end();
}

/**
* @param ve
 * @param viewNo
 * @return -1 : û���ҵ� other: λ��
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
 * �鿴msgs���������޶�Ӧ�����кŵ�msg
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
 * �㲥msg��Ϣ
 * @param msg
 */
void aloneNodeController::sendMsg(Msg* msg) {
    string type=msg->getType();
    putString(type+"���͵����ݿ�ʼ�㲥��");
}

/**
 * ����reply��ϵͳ
 * @param reply
 * @return
 */
int aloneNodeController::sendReply(Msg* reply) {
    putString("��ʼ����reply��Ϣ");
    return 0;
}

/**
 * ��msg�����Ӧ���͵Ĵ��������
 * @param node
 * @param msg
 */
void aloneNodeController::pushReadMsg(aloneNodeController*node, Msg* msg) {


    string msgType = msg->getType();
    putString("��Ϊ�ڵ㻹û���յ�ȷ����Ϣ���޷��ж��Լ��ܷ������԰�"+msgType+"���͵���Ϣ����������б�");
    if("request"==msgType){
        requestReadyList.push_back(msg);
        putString("request��Ϣ�ѷ���������б�,��ǰ��С"+to_string(requestReadyList.size()));
    }else if("ppMsg"==msgType){
        node->ppMsgReadyList.push_back(msg);
        putString("pp��Ϣ�ѷ���������б�,��ǰ��С"+to_string(node->ppMsgReadyList.size()));
    }else if("pMsg"==msgType){
        node->pMsgReadyList.push_back(msg);
        putString("p��Ϣ�ѷ���������б�,��ǰ��С"+to_string(node->pMsgReadyList.size()));
    }else if("commit"==msgType){
        node->cMsgReadyList.push_back(msg);
        putString("c��Ϣ�ѷ���������б�,��ǰ��С"+to_string(node->cMsgReadyList.size()));
    }

}

/**
 * ɾ��nodes���һ��ڵ㲢delete
 */
void aloneNodeController::removeHalfNodes() {
    int nodeSize=nodes.size();
    for (int i = 0; i < nodeSize/2; ++i) {
        delete nodes.at(i);
        nodes.erase(begin(nodes));
    }
}

/**
 * ���ppMsg�Ƿ����Ҫ��
 * @param ppMsg
 * @return
 */
bool aloneNodeController::checkPpMsg(Msg* ppMsg) {

    if (check(ppMsg)){
        ////����Ƿ��յ��ظ�pp��Ϣ
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
 * ���pMsg�Ƿ����Ҫ��
 * @param pMsg
 * @return
 */
bool aloneNodeController::checkPMsg(Msg* pMsg) {
    return check(pMsg);
}

/**
 * ���cMsg�Ƿ����Ҫ��
 * @param commit
 * @return
 */
bool aloneNodeController::checkCMsg(Msg *commit) {
    return check(commit);
}

/**
 * ���Msg�Ƿ����Ҫ��
 * @param ppMsg
 * @return
 */
bool aloneNodeController::check(Msg *msg) {
    return msg->getRemark()==getHashCode(msg->getContent())&&msg->getViewNo()==viewNo
           &&msg->getSerialNo()>=lowLine&&msg->getSerialNo()<=highLine;
}


////getter��setter
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
 * ������
 * @param msg
 * @param nodeNo
 */
void aloneNodeController::getMsg(Msg *msg,int nodeNo) {
    nodes.at(nodeNo)->action(msg);
}

/**
 * �޲ι��췽��
 */
aloneNodeController::aloneNodeController() {

}

void aloneNodeController::putString(string msg) {
    cout<<msg+"\n";
}

