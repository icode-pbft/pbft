#include <iostream>
#include "config/Config.h"
#include "msg/Msg.h"
#include "controller/system/SystemController.h"
#include "controller/aloneNodeController.h"
#include <thread>
#include "Mytest.h"
#include <mutex>
#include <map>
#include <sstream>
//#include <json/json.h>
#include <windows.h>
using namespace std;

int test1();
//int test1();
//
//static mutex mex;
//int threadTest(){
//
//    SystemController* systemController1=SystemController::getSystemController();
//    SystemController* systemController2=SystemController::getSystemController();
//    mex.lock();
//    Sleep(systemController1->getReplySize()*500);
//    systemController1->setReplySize(systemController1->getReplySize()+1);
//    cout<<"*****"<<systemController2->getReplySize()<<endl;
//    mex.unlock();
//
//    return 0;
//}

int i=0;

void staticTest(aloneNodeController* aloneNodeController1){
//    aloneNodeController1->setViewNo(aloneNodeController1->getViewNo()+1);
//    cout<<aloneNodeController1->getViewNo()<<endl;
    int t=i;
    i=1+t;
    cout<<to_string(i)+"  *";
}

void temp(){
    while (1){
        Sleep(1000);
        cout<<"*\n";
    }
}
void temp1(){
    while (1){
        Sleep(1000);
        cout<<"#\n";
    }
}

void temp2(){
    for (int j = 0; j < 10; ++j) {
        i++;
        cout<<to_string(i)+"   *";
    }
}



int threadTest1() {
    aloneNodeController::init(1);
    Msg* confirm=Msg::createConfirm(1,11,{1,2,3,4},2);
    Msg* request=Msg::createRequest("123",1);
    Msg* ppMsg1_1=Msg::createPpMsg("123",1,1,2);
    Msg* pMsg1_1=Msg::createPMsg("123",1,1,2);
    Msg* pMsg1_2=Msg::createPMsg("123",1,1,3);
    Msg* pMsg1_3=Msg::createPMsg("123",1,1,4);
    Msg* cMsg1_1=Msg::createCMsg("123",1,1,2);
    Msg* cMsg1_2=Msg::createCMsg("123",1,1,3);
    Msg* cMsg1_3=Msg::createCMsg("123",1,1,4);
    Msg* request1=Msg::createRequest("1234",1);
    Msg* ppMsg2_1=Msg::createPpMsg("123",2,1,2);
    Msg* pMsg2_1=Msg::createPMsg("1234",2,1,2);
    Msg* pMsg2_2=Msg::createPMsg("1234",2,1,3);
    Msg* pMsg2_3=Msg::createPMsg("1234",2,1,4);
    Msg* cMsg2_1=Msg::createCMsg("1234",2,1,2);
    Msg* cMsg2_2=Msg::createCMsg("1234",2,1,3);
    Msg* cMsg2_3=Msg::createCMsg("1234",2,1,4);



    thread th1(aloneNodeController::start,confirm);
    thread th2(aloneNodeController::start,request);
    thread th3(aloneNodeController::start,pMsg1_1);
    thread th4(aloneNodeController::start,pMsg1_2);
    thread th5(aloneNodeController::start,pMsg1_3);
    thread th6(aloneNodeController::start,cMsg1_1);
    thread th7(aloneNodeController::start,cMsg1_2);
    thread th8(aloneNodeController::start,cMsg1_3);
    thread th9(aloneNodeController::start,pMsg2_1);
    thread th10(aloneNodeController::start,pMsg2_2);
    thread th11(aloneNodeController::start,pMsg2_3);
    thread th12(aloneNodeController::start,cMsg2_1);
    thread th13(aloneNodeController::start,cMsg2_2);
    thread th14(aloneNodeController::start,cMsg2_3);
    thread th15(aloneNodeController::start,request1);
    thread th16(aloneNodeController::start,ppMsg1_1);
    thread th17(aloneNodeController::start,ppMsg2_1);
//    thread th9(aloneNodeController::start,cMsg3);
//    thread th10(aloneNodeController::start,cMsg3);
//    thread th11(aloneNodeController::start,cMsg3);
    th1.detach();
    th2.detach();
    th3.detach();
    th4.detach();
    th5.detach();
    th6.detach();
    th7.detach();
    th8.detach();
    th9.detach();
    th10.detach();
    th11.detach();
    th12.detach();
    th13.detach();
    th14.detach();
    th15.detach();
    th16.detach();
    th17.detach();
    ////发送p消息
    Sleep(500);

    return 0;
}


int main(){
    threadTest1();



    return 0;

//    vector<int> test;
//    for (int j = 0; j <5 ; ++j) {
//        test.push_back(j);
//    }
//
//
//    for (int i:test) {
//
//        if(i==1)
//        {
//
//             test.erase(find(test.begin(),test.end(),3));
//        }
//        cout<<"****"<<i<<endl;
//
//    }



}

//测试读取配置文件

int test1(){
//    RespondMain* respondMain=new RespondMain;
//    cout<<respondMain->getType();
//    return 0;
    int port;
    std::string ipAddress;
    std::string username;
    std::string password;
    const char ConfigFile[]= "../config/config.txt";
    Config configSettings(ConfigFile);

    port = configSettings.Read("port", 0);
    ipAddress = configSettings.Read("ipAddress", ipAddress);
    username = configSettings.Read("username", username);
    password = configSettings.Read("password", password);
    std::cout<<"port:"<<port<<std::endl;
    std::cout<<"ipAddress:"<<ipAddress<<std::endl;
    std::cout<<"username:"<<username<<std::endl;
    std::cout<<"password:"<<password<<std::endl;

    return 0;
}