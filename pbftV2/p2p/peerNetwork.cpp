//
// Created by t0106 on 2019/7/14.
//

#include <iostream>
#include <vector>
#include <thread>
#include <WinSock2.h>
#include <functional>
#include <string>

#include "peerNetwork.h"

#include "peerThread.h"

using std::cout;
using std::endl;
using std::thread;
using std::string;


peerNetwork::peerNetwork() {

    this->port = 8015;
    peerThreads = vector<peerThread>(0);
    peers = vector<string>(0);
}

peerNetwork::peerNetwork(int port) {
    this->port = static_cast<u_short>(port);
    peerThreads = vector<peerThread>(0);
    peers = vector<string>(0);
}

void peerNetwork::connectToPeer(string host, int port) {
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == SOCKET_ERROR) {
        cout << "peerNetWork line 36: Socket() error: " << WSAGetLastError() << endl;
        return;
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<u_short>(port));
    serverAddr.sin_addr.S_un.S_addr = inet_addr(host.data());

    int receiveValue;
    // 发送连接请求
    receiveValue = connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (receiveValue == INVALID_SOCKET) {
        cout << "peerNetWork line 50: socket " << host << ":" << port << " can't connected." << WSAGetLastError() << endl;
        return;
    } else {
        cout << "\npeerNetWork line 53: socket " << host << ":" << port << " connected.\n" << endl;
        peers.emplace_back(host + ":" + std::to_string(port));

        peerThread pt = peerThread(clientSocket, serverAddr);

//        thread ptThread(&peerThread::run, std::ref(pt));
        thread ptThread(&peerThread::run, pt);

        ptThread.detach();

//        peerThreads.emplace_back(pt);
    }
}

void peerNetwork::run() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == SOCKET_ERROR) {
        cout << "peerNetWork line 69: Socket() error: " << WSAGetLastError() << endl;
        return;
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<u_short>(port));
    /*
     * INADDR_ANY就是指定地址为0.0.0.0的地址，
     * 这个地址事实上表示不确定地址，或“所有地址”、“任意地址”。
     * 一般来说，在各个系统中均定义成为0值。
     */
    serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    int receiveValue;
    // socket绑定端口
    receiveValue = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(SOCKADDR_IN));
    if (receiveValue == SOCKET_ERROR) {
        cout << "peerNetWork line 87: Failed bind: " << WSAGetLastError() << endl;
        cout << "peerNetWork line 88: 绑定端口失败，可能已被占用： " << WSAGetLastError() << endl;
        return;
    }

    // socket监听端口
    // 第二个参数backlog还不知道是什么意思
    receiveValue = listen(serverSocket, 10);
    if (receiveValue == SOCKET_ERROR) {
        cout << "peerNetWork line 96: Failed listen: " << WSAGetLastError() << endl;
        cout << "peerNetWork line 97: 监听端口失败： " << WSAGetLastError() << endl;
        return;
    }

    while (runFlag) {
        // 监听成功，等待Client端连接
        SOCKADDR_IN clientAddr;
        int lenSOCKADDR = sizeof(SOCKADDR);
        SOCKET connectSocket = accept(serverSocket, (SOCKADDR *) &clientAddr, &lenSOCKADDR);

        if (connectSocket == SOCKET_ERROR) {
            cout << "peerNetWork line 108: Failed accept: " << WSAGetLastError() << endl;
            cout << "peerNetWork line 109: Accept失败： " << WSAGetLastError() << endl;
        }

        cout << "\npeerNetWork line 112: Accept client IP: " << inet_ntoa(clientAddr.sin_addr) << "\n"<< endl;


        peerThread peerThread1 = peerThread(connectSocket, clientAddr);
        peerThreads.emplace_back(peerThread1);
        cout << peerThreads.size() << endl;
//        thread ptThread(&peerThread::run, std::ref(peerThread1));
        thread ptThread(&peerThread::run, peerThread1);
        ptThread.detach();
    }
}

void peerNetwork::broadcast(string data) {
    for (auto pt: peerThreads) {
        cout << "sent: " << data << endl;
        pt.send(data);
    }
}
