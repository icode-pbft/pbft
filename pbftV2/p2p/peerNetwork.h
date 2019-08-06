//
// Created by t0106 on 2019/7/14.
//

#ifndef TCP_TEST_PEERNETWORK_H
#define TCP_TEST_PEERNETWORK_H

#include <iostream>
#include <vector>

#include "peerThread.h"

using std::string;
using std::vector;

/**
 * P2P网络，负责处理peer之间的通信
 */
class peerNetwork {

private:
    int port;
    bool runFlag = true;

public:
    vector<peerThread> peerThreads;
    vector<string> peers;

    /**
     * 默认配置
     */
    peerNetwork();

    explicit peerNetwork(int port);

    /**
     * 建立连接
     */
    void connectToPeer(string host, int port);

    void run();

    void broadcast(string data);
};


#endif //TCP_TEST_PEERNETWORK_H
