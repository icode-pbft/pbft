#include <sstream>
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
    result["result"] = this->result;
    result["systemIp"] = this->systemIp;
    return result;
}

string Msg::toJsonStr() {
    // 2019年8月2日15点33分 谭强
    // 序列化成无缩进格式
    Json::StreamWriterBuilder builder;
    builder.settings_["indentation"] = "";

    Json::StreamWriter *writer(builder.newStreamWriter());
    std::ostringstream os;

    writer->write(toJsonValue(), &os);

    return os.str();
}

Msg* Msg::fromJson(const string& itemStr) {
    Json::Value jsonItem;
    Json::CharReaderBuilder builder;
    Json::CharReader* reader(builder.newCharReader());
    vector<int> choosed;
    JSONCPP_STRING errors;

    Msg *msg = new Msg();

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
        msg->setResult(jsonItem["result"].asString());
        msg->setSystemIp(jsonItem["systemIp"].asInt());
    } else {
        std::cout << errors << std::endl;
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

Msg::Msg(const string &type, int serialNo, string result) :
        type(type), serialNo(serialNo),result(result) {}

Msg::Msg() {}

const string &Msg::getRemark() const {
    return remark;
}

void Msg::setRemark(const string &remark) {
    Msg::remark = remark;
}

const string &Msg::getResult() const {
    return result;
}

void Msg::setResult(const string &result) {
    Msg::result = result;
}

int Msg::getSystemIp() const {
    return systemIp;
}

void Msg::setSystemIp(int systemIp) {
    Msg::systemIp = systemIp;
}