#include "aloneNodeController.h"
#include "../pbftMain.h"
#include "../log/logger.h"


/// �������ڵ��viewNo����ģ��ж��ٸ���ͬ��viewNo���ж��ٸ�controller��
/// Ϊ�������ֹ�ʶδ���������µ��������ʱ��������
vector<aloneNodeController *> aloneNodeController::nodes;
vector<Msg *> aloneNodeController::requestReadyList;
const int aloneNodeController::nodeSizeMax = 100;
int aloneNodeController::nodeNo;

/**
 * ��ʶ��ʼ
 * @param msg
 */
void aloneNodeController::start(Msg *msg) {
    string msgType = msg->getType();

    if (msgType == "request") {
        int viewNo = msg->getViewNo();
        for (aloneNodeController *node:nodes) {
            if (viewNo == node->getViewNo()) {
                node->action(msg);
                return;
            }
        }

        /// ��������ͼ�Ŷ�Ӧ�Ľڵ㣬��request�������������
        requestReadyList.push_back(msg);

    } else if (msgType == "confirmMain") {
        if (nodes.size() >= nodeSizeMax) {
            removeHalfNodes();
        }

        int index = nodeIn(nodes, msg->getViewNo());

        /// ���nodes�����ﲻ���ڸ���ͼ�Ŷ�Ӧ�Ľڵ�
        if (-1 == index) {
            auto *node = new aloneNodeController();
            node->setViewNo(msg->getViewNo());
            nodes.push_back(node);
            node->action(msg);

            /// ����������������request
            for (auto &i : requestReadyList) {
                if (i->getViewNo() == node->getViewNo()) {
                    node->action(i);
                }
            }
        } else {
            nodes.at(index)->action(msg);
        }
    } else {
        /// �鿴�Ƿ��Ѿ���������ͼ�Ŷ�Ӧ�Ľڵ�
        int index = nodeIn(nodes, msg->getViewNo());
        if (-1 == index) {
            return;
        }
        nodes.at(index)->action(msg);
    }
}

/**
 * ɾ��nodes���һ��ڵ㲢delete
 */
void aloneNodeController::removeHalfNodes() {
    int nodeSize = nodes.size();
    for (int i = 0; i < nodeSize / 2; ++i) {
        delete nodes.at(i);
        nodes.erase(begin(nodes));
    }
}
/**
 * �յ���Ϣ�Լ���Ӧ����
 * @param msg
 */
void aloneNodeController::action(Msg *msg) {
     string msgType = msg->getType();

    if (msgType == "confirmMain") {
        dealWithConfirm(msg);
    } else if (!isVote) {
        return;
    } else if (msgType == "ppMsg") {
        dealWithPpMsg(msg);
    } else if (msgType == "pMsg") {
        dealWithPMsg(msg);
    } else if (msgType == "cMsg") {
        dealWithCMsg(msg);
    } else if (msgType == "request") {
        dealWithRequest(msg);
    }
}

/**
 * ����Confirm��Ϣ
 * @param confirmMain
 */
void aloneNodeController::dealWithConfirm(Msg *confirmMain) {
    this->setViewNo(confirmMain->getViewNo());
    vector<int> chooseNodes = confirmMain->getChooseNodes();

    initConfig();
    highLine = configSettings.Read("highLine", 0);
    lowLine = configSettings.Read("lowLine", 0);

    chooseSize = confirmMain->getChooseNodes().size();
    systemIp = confirmMain->getSystemIp();

    /// �鿴�ýڵ��Ƿ���ͶƱ�ڵ�
    vector<int>::iterator ret;
    ret = std::find(chooseNodes.begin(), chooseNodes.end(), nodeNo);

    if (ret != chooseNodes.end()) {
        string temp = "ȷ����ݣ�ͶƱ�ڵ�ID��" + to_string(nodeNo) + ", ���ڵ��ţ� " + to_string(confirmMain->getMainNode())
                + ", ��ͼ��ţ� " + to_string(confirmMain->getViewNo());
        writeLog(temp);
        isVote = true;
    } else {
        isVote = false;
        return;
    }
}

/**
 * ����request��Ϣ
 * @param request
 */
void aloneNodeController::dealWithRequest(Msg *request) {
    pTimes[times][0] = serialNo;
    cTimes[times][0] = serialNo;

    string temp = "request�׶�: ���ڵ�ID: " + to_string(request->getMainNode());

    writeLog(temp);


    auto *ppMsg = new Msg();
    ppMsg->setType("ppMsg");
    ppMsg->setViewNo(viewNo);
    ppMsg->setContent(request->getContent());
    ppMsg->setRemark(request->getRemark());
    ppMsg->setSerialNo(serialNo);

    ppMsgList.push_back(serialNo);
    serialNo++;
    times++;

    sendMsg(ppMsg);

}

/**
 * ����pp��Ϣ
 * @param ppMsg
 */
void aloneNodeController::dealWithPpMsg(Msg *ppMsg) {
    if (!checkPpMsg(ppMsg)) {
        return;
    } else {
        int serialNo = ppMsg->getSerialNo();
        ppMsgs.push_back(ppMsg);
        pTimes[times][0] = serialNo;
        cTimes[times][0] = serialNo;
        ppMsgList.push_back(ppMsg->getSerialNo());

        string temp = "pp�׶�: ͶƱ���ID: " + to_string(nodeNo) + " , ��ͼ:" + to_string(ppMsg->getViewNo())
                + " , ���к�:" + to_string(ppMsg->getSerialNo()) + " , ժҪ:" + ppMsg->getRemark();

        writeLog(temp);

        /// ��¼���к�
        serialNo = ppMsg->getSerialNo();
        Msg *pMsg = new Msg("pMsg", viewNo, ppMsg->getRemark(), ppMsg->getSerialNo(), nodeNo, ppMsg->getContent());
        sendMsg(pMsg);

        for (Msg *msg:pMsgReadyList) {
            if (serialNo == msg->getSerialNo()) {

                dealWithPMsg(msg);
            }
        }
    }
    times++;
}

/**
 * ����P��Ϣ
 * @param pMsg
 */
void aloneNodeController::dealWithPMsg(Msg *pMsg) {
    if (!checkPMsg(pMsg)) {
        return;
    } else {
        int preSerialNo = pMsg->getSerialNo();

        /// ���Ѵ����pp��Ϣ��û�ж�Ӧ�����к�ʱ
        if(!somethingIn(ppMsgList, preSerialNo)){
            pMsgReadyList.push_back(pMsg);
            return;
        }

        for (int i = 0; i < times + 1; i++) {
            if (pTimes[i][0] == preSerialNo) {
                pTimes[i][1] += 1;

                if (pTimes[i][1] == chooseSize / 3 * 2) {
                    pMsgList.push_back(pMsg->getSerialNo());

                    string temp = "p�׶�: ͶƱ���ID: " + to_string(nodeNo) + " , ��ͼ:" + to_string(pMsg->getViewNo())
                            + " , ���к�:" + to_string(pMsg->getSerialNo()) + " , ժҪ:" + pMsg->getRemark();
                    writeLog(temp);

                    auto *commit = new Msg("cMsg", viewNo, pMsg->getRemark(), preSerialNo, nodeNo);
                    commit->setContent(pMsg->getContent());
                    sendMsg(commit);
                }
                return;
            }
        }

    }
}

/**
 * ����C��Ϣ
 * @param commit
 */
void aloneNodeController::dealWithCMsg(Msg *commit) {
    if (!checkCMsg(commit)) {
        return;
    } else {
        int commitSerialNo = commit->getSerialNo();

        /// ���Ѵ����p��Ϣ��û�ж�Ӧ�����к�ʱ
        if(!somethingIn(ppMsgList, commitSerialNo)){
            pMsgReadyList.push_back(commit);
            return;
        }

        for (int i = 0; i < times + 1; i++) {
            if (cTimes[i][0] == commitSerialNo) {
                cTimes[i][1] += 1;

                if (cTimes[i][1] == chooseSize / 3 * 2) {
                    cMsgList.push_back(commitSerialNo);


                    string temp = "c�׶�: ͶƱ���ID: " + to_string(nodeNo) + " , ��ͼ:" + to_string(commit->getViewNo())
                            + " , ���к�:" + to_string(commit->getSerialNo()) + " , ժҪ:" + commit->getRemark();
                    writeLog(temp);

                    string result = "reply result test";
                    auto *reply = new Msg("Reply", commitSerialNo, result);
                    reply->setSystemIp(this->getSystemIp());
                    sendReply(reply);
                }
                return;
            }
        }
    }
}

/**
 * pp�׶μ���յ�����Ϣ�����к��Լ���ͼ���Ƿ���ϱ�׼
 * @param ppMsg
 * @return ����Ϊtrue
 */
bool aloneNodeController::checkPpMsg(Msg *ppMsg) {
    if (check(ppMsg)) {
        for (auto &i : ppMsgs) {
            if (i->getSerialNo() == ppMsg->getSerialNo() &&
                i->getViewNo() == ppMsg->getViewNo()) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

/**
 * p�׶���Ϣ����յ�����Ϣ��ժҪ����ͼ�š����к��Ƿ���ϱ�׼
 * @param pMsg
 * @return
 */
bool aloneNodeController::checkPMsg(Msg *pMsg) {
    return check(pMsg);
}

/**
 * C�׶���Ϣ����յ�����Ϣ��ժҪ����ͼ�š����к��Ƿ���ϱ�׼
 * @param commit
 * @return
 */
bool aloneNodeController::checkCMsg(Msg *commit) {
    return check(commit);
}

/**
 * �����Ϣ
 * @param ppMsg
 * @return
 */
bool aloneNodeController::check(Msg *ppMsg) {
    string hashCode = to_string(getHashCode(ppMsg->getContent()));

    return ppMsg->getRemark() == hashCode && ppMsg->getViewNo() == viewNo
           && ppMsg->getSerialNo() <= highLine && ppMsg->getSerialNo() >= lowLine;
}

/**
 * �ڵ㷢��Ϣ
 * @param msg
 */
void aloneNodeController::sendMsg(Msg *msg) {
    pbftMain::broadcast(msg->toJsonStr());
}

/**
 * �ڵ���ϵͳ����reply��Ϣ
 * @param reply
 */
void aloneNodeController::sendReply(Msg *reply) {
    pbftMain::sendToNode(reply->toJsonStr(), reply->getSystemIp());
}

/**
 * ����ַ�����hashֵ
 * @param content
 * @return ��ϣֵ��ʮ�����޷���ֵ
 */
size_t aloneNodeController::getHashCode(string content) {
    hash<string> hashTest;
    return hashTest(content);
}

/**
 * ��ȡ�����ļ���ʼ������
 */
void aloneNodeController::initConfig() {
    const char ConfigFile[] = "../config/config.txt";
    Config config(ConfigFile);
    configSettings = config;

    initLogger("../log/info.txt", "../log/warn.txt", "../log/error.txt");
}

/**
 * ��Ӧ���������Ƿ��и�int����
 * @param ve
 * @param num
 * @return
 */
bool aloneNodeController::somethingIn(vector<int> ve, int num) {
    return std::find(ve.begin(), ve.end(), num) != ve.end();
}

/**
 * ���ڵ��б��ж�Ӧ��ͼ�ŵĽڵ��λ�ã��������򷵻�-1
 * @param ve
 * @param viewNo
 * @return
 */
int aloneNodeController::nodeIn(vector<aloneNodeController *> ve, int viewNo) {
    for (int i = 0; i < ve.size(); ++i) {
        if (ve.at(i)->getViewNo() == viewNo) {
            return i;
        }
    }
    return -1;
}

/**
 * д��־
 * @param message
 */
void aloneNodeController::writeLog(string message) {
    LOG(INFO) << message;
}

int aloneNodeController::getViewNo() const {
    return viewNo;
}

void aloneNodeController::setViewNo(int viewNo) {
    aloneNodeController::viewNo = viewNo;
}

void aloneNodeController::setNodeNo(int nodeNo) {
    aloneNodeController::nodeNo = nodeNo;
}

int aloneNodeController::getSystemIp() const {
    return systemIp;
}
