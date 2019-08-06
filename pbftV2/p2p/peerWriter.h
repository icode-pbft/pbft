//
// Created by t0106 on 2019/7/14.
//

#ifndef TCP_TEST_PEERWRITER_H
#define TCP_TEST_PEERWRITER_H

#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <queue>
#include <map>
#include <mutex>
using std::string;
using std::vector;
using std::cout;
using std::endl;


class peerWriter {
private:
    SOCKET clientSocket;
    SOCKADDR_IN serverAddr;
    //fileName=Ip
    std::string fileName;
public:
    /**
     * 2019??8??3??09:59:00 ?????§Õ???map<>???key?IP,value???????§µ?????§Õ?????
     * */
    static std::map<std::string ,std::queue<std::string>> writeMap;

    static std::mutex writeMapMutex;
private:

    // todo: ?????
    vector<string> outputBuffer = vector<string>(0);

    bool runFlag = true;
public:
    peerWriter();

    peerWriter(SOCKET clientSocket, SOCKADDR_IN serverAddr);
    //??????
    peerWriter(SOCKET clientSocket, const SOCKADDR_IN &serverAddr, const string &fileName);
    void run();

    void write(string msg);

};


#endif //TCP_TEST_PEERWRITER_H
