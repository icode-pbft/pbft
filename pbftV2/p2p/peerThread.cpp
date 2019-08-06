//
// Created by t0106 on 2019/7/14.
//
#include <thread>
#include <functional>

#include "peerThread.h"

using std::thread;

/**
 * p2p通讯线程
 * 在接受套接字后，分成两个独立的线程，一个用于输入数据，一个用于输出数据，因此单向数据不会阻塞
 *
 * @author Mignet
 */
// C++是真的?
//peerThread::peerThread(SOCKET clientSocket, const SOCKADDR_IN &serverAddr) :
//peerReader1(clientSocket),
//peerWriter1(clientSocket, serverAddr)
//{
//    this->clientSocket = clientSocket;
//    this->serverAddr = serverAddr;
//}

peerThread::peerThread(SOCKET clientSocket, const SOCKADDR_IN &serverAddr)
{
    this->clientSocket = clientSocket;
    this->serverAddr = serverAddr;

    this->fileName = inet_ntoa(serverAddr.sin_addr);
    this->peerWriter1 = peerWriter(clientSocket, serverAddr, fileName);
    this->peerReader1 = peerReader(clientSocket, fileName);
}


void peerThread::run() {
    cout << "\npeerThread line 36: Got connection from " << inet_ntoa(serverAddr.sin_addr) << endl;

    // 心理慌得很，完全不知道能不能跑起来
//    thread peerReaderThread(&peerReader::run, std::ref(peerReader1));
    thread peerReaderThread(&peerReader::run, peerReader1);
    peerReaderThread.detach();

//    thread peerWriterThread(&peerWriter::run, std::ref(peerWriter1));
    thread peerWriterThread(&peerWriter::run, peerWriter1);
    peerWriterThread.detach();
}

void peerThread::send(string data) {
    // 在Java中要对peerWrite1进行判空，而C++不一样，不能指向空指针，哭了
    // Clang-Tidy: Parameter 'data' is passed by value and only copied once;
    // consider moving it to avoid unnecessary copies
    // 不晓得囊个解决这个warning，C++??
    peerWriter1.write(data);
}



peerThread::peerThread() {}

