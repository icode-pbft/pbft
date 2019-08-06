//
// Created by t0106 on 2019/7/14.
//
#include <thread>
#include <string>
#include <fstream>
#include <mutex>

#include "peerWriter.h"
#include "../utils/commonUtils.h"

std::map<string ,std::queue<string>> peerWriter::writeMap;
std::mutex peerWriter::writeMapMutex;

peerWriter::peerWriter(SOCKET clientSocket, SOCKADDR_IN serverAddr) {
    this->clientSocket = clientSocket;
    this->serverAddr = serverAddr;
}

void peerWriter::run() {
    while (runFlag) {
        std::string sendMsg;

        writeMapMutex.lock();
        if (!peerWriter::writeMap[this->fileName].empty()) {
//            std::lock_guard<std::mutex> lcg(writeMapMutex);
            sendMsg=peerWriter::writeMap[this->fileName].front();
            peerWriter::writeMap[this->fileName].pop();
        }
        writeMapMutex.unlock();
        // todo:改成读取map中的队列第一个消息
//        commonUtils::readLineFile(fileName, sendMsg);

        if (!sendMsg.empty()) {
            sendMsg += "\r\n";
            if (send(clientSocket, sendMsg.data(), static_cast<int>(sendMsg.size()), 0) < 0) {
                std::cout << "sending ERROR!!! -->send " + sendMsg + " to " +  inet_ntoa(serverAddr.sin_addr) << std::endl;
            } else {
                std::cout << "sending success -->send " + sendMsg + " to " +  inet_ntoa(serverAddr.sin_addr) << std::endl;
            }
            sendMsg = "";
            //todo:将清空文件内容改成弹出第一个消息
//            std::ofstream fileout(fileName, std::ios::out | std::ios::trunc);
//            fileout.close();
        }

        // todo:线程沉睡
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    cout << "Peer " << inet_ntoa(serverAddr.sin_addr) << " disconnected.";
}

/**
 * 发送消息
 */
void peerWriter::write(string msg) {
    //todo:改写为存入map中队列的末尾
//    commonUtils::writeToFile(fileName, msg, std::ios::out | std::ios::trunc);
//    存入map中队列的末尾
    writeMapMutex.lock();
    peerWriter::writeMap[this->fileName].push(msg);
    writeMapMutex.unlock();
}

peerWriter::peerWriter() {}

/**
 *构造函数 初始时即给static map赋予key值
 */
peerWriter::peerWriter(SOCKET clientSocket, const SOCKADDR_IN &serverAddr, const string &fileName) : clientSocket(
        clientSocket), serverAddr(serverAddr), fileName(fileName) {
    std::queue<string> writeQueue;
    peerWriter::writeMap[this->fileName]=writeQueue;
}


