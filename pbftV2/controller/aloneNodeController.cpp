#include "aloneNodeController.h"
#include "../pbftMain.h"
#include "../log/logger.h"


/// 更换主节点后viewNo会更改，有多少个不同的viewNo就有多少个controller，
/// 为避免六轮共识未进行完有新的请求进来时阻塞过久
vector<aloneNodeController *> aloneNodeController::nodes;
vector<Msg *> aloneNodeController::requestReadyList;
const int aloneNodeController::nodeSizeMax = 100;
int aloneNodeController::nodeNo;

/**
 * 共识开始
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

        /// 不存在视图号对应的节点，将request存入待处理向量
        requestReadyList.push_back(msg);

    } else if (msgType == "confirmMain") {
        if (nodes.size() >= nodeSizeMax) {
            removeHalfNodes();
        }

        int index = nodeIn(nodes, msg->getViewNo());

        /// 如果nodes向量里不存在该视图号对应的节点
        if (-1 == index) {
            auto *node = new aloneNodeController();
            node->setViewNo(msg->getViewNo());
            nodes.push_back(node);
            node->action(msg);

            /// 处理待处理向量里的request
            for (auto &i : requestReadyList) {
                if (i->getViewNo() == node->getViewNo()) {
                    node->action(i);
                }
            }
        } else {
            nodes.at(index)->action(msg);
        }
    } else {
        /// 查看是否已经生成了视图号对应的节点
        int index = nodeIn(nodes, msg->getViewNo());
        if (-1 == index) {
            return;
        }
        nodes.at(index)->action(msg);
    }
}

/**
 * 删除nodes里的一半节点并delete
 */
void aloneNodeController::removeHalfNodes() {
    int nodeSize = nodes.size();
    for (int i = 0; i < nodeSize / 2; ++i) {
        delete nodes.at(i);
        nodes.erase(begin(nodes));
    }
}
/**
 * 收到消息以及对应处理
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
 * 处理Confirm消息
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

    /// 查看该节点是否是投票节点
    vector<int>::iterator ret;
    ret = std::find(chooseNodes.begin(), chooseNodes.end(), nodeNo);

    if (ret != chooseNodes.end()) {
        string temp = "确认身份：投票节点ID：" + to_string(nodeNo) + ", 主节点编号： " + to_string(confirmMain->getMainNode())
                + ", 视图编号： " + to_string(confirmMain->getViewNo());
        writeLog(temp);
        isVote = true;
    } else {
        isVote = false;
        return;
    }
}

/**
 * 处理request消息
 * @param request
 */
void aloneNodeController::dealWithRequest(Msg *request) {
    pTimes[times][0] = serialNo;
    cTimes[times][0] = serialNo;

    string temp = "request阶段: 主节点ID: " + to_string(request->getMainNode());

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
 * 处理pp消息
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

        string temp = "pp阶段: 投票结点ID: " + to_string(nodeNo) + " , 视图:" + to_string(ppMsg->getViewNo())
                + " , 序列号:" + to_string(ppMsg->getSerialNo()) + " , 摘要:" + ppMsg->getRemark();

        writeLog(temp);

        /// 记录序列号
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
 * 处理P消息
 * @param pMsg
 */
void aloneNodeController::dealWithPMsg(Msg *pMsg) {
    if (!checkPMsg(pMsg)) {
        return;
    } else {
        int preSerialNo = pMsg->getSerialNo();

        /// 当已处理的pp消息里没有对应的序列号时
        if(!somethingIn(ppMsgList, preSerialNo)){
            pMsgReadyList.push_back(pMsg);
            return;
        }

        for (int i = 0; i < times + 1; i++) {
            if (pTimes[i][0] == preSerialNo) {
                pTimes[i][1] += 1;

                if (pTimes[i][1] == chooseSize / 3 * 2) {
                    pMsgList.push_back(pMsg->getSerialNo());

                    string temp = "p阶段: 投票结点ID: " + to_string(nodeNo) + " , 视图:" + to_string(pMsg->getViewNo())
                            + " , 序列号:" + to_string(pMsg->getSerialNo()) + " , 摘要:" + pMsg->getRemark();
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
 * 处理C消息
 * @param commit
 */
void aloneNodeController::dealWithCMsg(Msg *commit) {
    if (!checkCMsg(commit)) {
        return;
    } else {
        int commitSerialNo = commit->getSerialNo();

        /// 当已处理的p消息里没有对应的序列号时
        if(!somethingIn(ppMsgList, commitSerialNo)){
            pMsgReadyList.push_back(commit);
            return;
        }

        for (int i = 0; i < times + 1; i++) {
            if (cTimes[i][0] == commitSerialNo) {
                cTimes[i][1] += 1;

                if (cTimes[i][1] == chooseSize / 3 * 2) {
                    cMsgList.push_back(commitSerialNo);


                    string temp = "c阶段: 投票结点ID: " + to_string(nodeNo) + " , 视图:" + to_string(commit->getViewNo())
                            + " , 序列号:" + to_string(commit->getSerialNo()) + " , 摘要:" + commit->getRemark();
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
 * pp阶段检查收到的消息其序列号以及视图号是否符合标准
 * @param ppMsg
 * @return 符合为true
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
 * p阶段消息检查收到的消息其摘要、视图号、序列号是否符合标准
 * @param pMsg
 * @return
 */
bool aloneNodeController::checkPMsg(Msg *pMsg) {
    return check(pMsg);
}

/**
 * C阶段消息检查收到的消息其摘要、视图号、序列号是否符合标准
 * @param commit
 * @return
 */
bool aloneNodeController::checkCMsg(Msg *commit) {
    return check(commit);
}

/**
 * 检查消息
 * @param ppMsg
 * @return
 */
bool aloneNodeController::check(Msg *ppMsg) {
    string hashCode = to_string(getHashCode(ppMsg->getContent()));

    return ppMsg->getRemark() == hashCode && ppMsg->getViewNo() == viewNo
           && ppMsg->getSerialNo() <= highLine && ppMsg->getSerialNo() >= lowLine;
}

/**
 * 节点发消息
 * @param msg
 */
void aloneNodeController::sendMsg(Msg *msg) {
    pbftMain::broadcast(msg->toJsonStr());
}

/**
 * 节点向系统发送reply消息
 * @param reply
 */
void aloneNodeController::sendReply(Msg *reply) {
    pbftMain::sendToNode(reply->toJsonStr(), reply->getSystemIp());
}

/**
 * 获得字符串的hash值
 * @param content
 * @return 哈希值的十进制无符号值
 */
size_t aloneNodeController::getHashCode(string content) {
    hash<string> hashTest;
    return hashTest(content);
}

/**
 * 读取配置文件初始化参数
 */
void aloneNodeController::initConfig() {
    const char ConfigFile[] = "../config/config.txt";
    Config config(ConfigFile);
    configSettings = config;

    initLogger("../log/info.txt", "../log/warn.txt", "../log/error.txt");
}

/**
 * 对应的向量中是否有该int类型
 * @param ve
 * @param num
 * @return
 */
bool aloneNodeController::somethingIn(vector<int> ve, int num) {
    return std::find(ve.begin(), ve.end(), num) != ve.end();
}

/**
 * 检查节点列表中对应视图号的节点的位置，不存在则返回-1
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
 * 写日志
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
