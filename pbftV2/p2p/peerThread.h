//
// Created by t0106 on 2019/7/14.
//

#ifndef TCP_TEST_PEERTHREAD_H
#define TCP_TEST_PEERTHREAD_H

#include <WinSock2.h>

#include "peerWriter.h"
#include "peerReader.h"

using std::cout;
using std::endl;

class peerThread {
private:
    SOCKET clientSocket;
    SOCKADDR_IN serverAddr;

public:
    // = ip
    // ex: 192.168.10.10
    std::string fileName;
    peerWriter peerWriter1 = peerWriter();
    peerReader peerReader1 = peerReader();

public:
    peerThread();

    peerThread(SOCKET clientSocket, const SOCKADDR_IN &serverAddr);

    void run();

    void send(string data);
};


#endif //TCP_TEST_PEERTHREAD_H
