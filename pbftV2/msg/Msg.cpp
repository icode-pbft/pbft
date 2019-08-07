#include "Msg.h"

Json::Value Msg::toJsonValue() {
    Json::Value result;
    Json::Value temp;
    result["type"] = type;
    result["viewNo"] = viewNo;
    for(int i = 0; i < chooseNodes.size(); i++){
        temp[i] = chooseNodes[i];
    }
    result["chooseNodes"] = temp;
    result["mainNode"] = mainNode;
    result["remark"] = remark;
    result["serialNo"] = serialNo;
    result["nodeNo"] = nodeNo;
    result["content"] = content;
    result["result"] =this->result;
    return result;
}

string Msg::toJsonStr() {
    return toJsonValue().toStyledString();
}

Msg* Msg::fromJson(const string& itemStr) {
    Json::Value jsonItem;
    Json::CharReaderBuilder builder;
    Json::CharReader* reader(builder.newCharReader());
    vector<int> choosed;
    JSONCPP_STRING errors;

    Msg* msg=new Msg();
    if(reader->parse(itemStr.data(), itemStr.data() + itemStr.size(), &jsonItem, &errors)){
        msg->setType(jsonItem["type"].asString());
        msg->setViewNo(jsonItem["viewNo"].asInt());
        Json::Value choose = jsonItem["chooseNodes"];
        for(unsigned int i = 0; i < choose.size(); i++){
            choosed.push_back(choose[i].asInt());
        }
        msg->setChooseNodes(choosed);
        msg->setMainNode(jsonItem["mainNo"].asInt());
        msg->setRemark(jsonItem["remark"].asString());
        msg->setSerialNo(jsonItem["serialNo"].asInt());
        msg->setNodeNo(jsonItem["nodeNo"].asInt());
        msg->setContent(jsonItem["content"].asString());
        msg->setResult(jsonItem["result"].asBool());
    }
    return msg;
}

const string &Msg::getType() const {
    return type;
}

void Msg::setType(const string &type) {
    Msg::type = type;
}

int Msg::getViewNo() const {
    return viewNo;
}

void Msg::setViewNo(int viewNo) {
    Msg::viewNo = viewNo;
}

const vector<int> &Msg::getChooseNodes() const {
    return chooseNodes;
}

void Msg::setChooseNodes(const vector<int> &chooseNodes) {
    Msg::chooseNodes = chooseNodes;
}

int Msg::getMainNode() const {
    return mainNode;
}

void Msg::setMainNode(int mainNode) {
    Msg::mainNode = mainNode;
}

int Msg::getSerialNo() const {
    return serialNo;
}

void Msg::setSerialNo(int serialNo) {
    Msg::serialNo = serialNo;
}

int Msg::getNodeNo() const {
    return nodeNo;
}

void Msg::setNodeNo(int nodeNo) {
    Msg::nodeNo = nodeNo;
}

const string &Msg::getContent() const {
    return content;
}

void Msg::setContent(const string &content) {
    Msg::content = content;
}

Msg::Msg(const string &type, int viewNo, string remark, int serialNo,
         int nodeNo, const string &content) : type(type), viewNo(viewNo), remark(remark), serialNo(serialNo),
                                              nodeNo(nodeNo), content(content) {}

Msg::Msg(const string &type, int viewNo, string remark, int serialNo,int nodeNo) :
        type(type), viewNo(viewNo), remark(remark), serialNo(serialNo),nodeNo(nodeNo) {}

Msg::Msg(const string &type, int serialNo, bool result) :
        type(type), serialNo(serialNo),result(result) {}

Msg::Msg() {}

const string &Msg::getRemark() const {
    return remark;
}

void Msg::setRemark(const string &remark) {
    Msg::remark = remark;
}

const bool &Msg::getResult() const {
    return result;
}

void Msg::setResult(const bool &result) {
    Msg::result = result;
}

int Msg::getSystemId() const {
    return systemId;
}

void Msg::setSystemId(int systemId) {
    Msg::systemId = systemId;
}


/**
 *
 * @param viewNo
 * @param chooseNodes
 * @param mainNode
 * @param systemId
 * @param type
 */
Msg::Msg(int viewNo, const vector<int> &chooseNodes, int mainNode, int systemId,const string& type) : viewNo(viewNo),
                                                                                   chooseNodes(chooseNodes),
                                                                                   mainNode(mainNode),
                                                                                   systemId(systemId),
                                                                                   type(type)
                                                                                   {}
/**
 *
 * @param viewNo
 * @param serialNo
 * @param content
 * @param type
 */
Msg::Msg(int viewNo, int serialNo, const string &content,const string &type,const string &remark,int nodeNo) : viewNo(viewNo),
serialNo(serialNo),
content(content),
type(type),
remark(remark),
nodeNo(nodeNo){}


/**
 *
 * @param viewNo
 * @param serialNo
 * @param nodeNo
 * @param result
 * @param type
 */
Msg::Msg(int viewNo, int serialNo, int nodeNo, bool result,const string &type) : viewNo(viewNo), serialNo(serialNo), nodeNo(nodeNo),
                                                              result(result), type(type) {}

/**
 *
 * @param type
 * @param viewNo
 * @param content
 */
Msg::Msg(const string &type, int viewNo, const string &content,const string &remark) : type(type), viewNo(viewNo), content(content) ,remark(remark){}



string Msg::getHashCode(string content) {
    hash<string> hashTest;
    return to_string(hashTest(content));
}


Msg *Msg::createConfirm(int viewNo, int systemId, vector<int> chooseNodes, int mainNode) {

    return  new Msg(viewNo,chooseNodes,mainNode,systemId,"confirm");
}

Msg *Msg::createRequest(string content, int viewNo) {
    return new Msg("request",viewNo,content,getHashCode(content));
}

Msg *Msg::createPpMsg(string content, int serialNo, int viewNo,int nodeNo) {
    return new Msg(viewNo,serialNo,content,"ppMsg",getHashCode(content),nodeNo);
}

Msg *Msg::createPMsg(string content, int serialNo, int viewNo,int nodeNo) {
    return new Msg(viewNo,serialNo,content,"pMsg",getHashCode(content),nodeNo);
}

Msg *Msg::createCMsg(string content, int serialNo, int viewNo,int nodeNo) {
    return new Msg(viewNo,serialNo,content,"commit",getHashCode(content),nodeNo);
}

Msg *Msg::createReply(bool result, int serialNo, int viewNo, int nodeNo) {
    return new Msg(viewNo,serialNo,nodeNo,result,"reply");
}

