//
// Created by t0106 on 2019/7/14.
//

#include <fstream>
#include <mutex>
#include <string>

#include "peerReader.h"


std::map<std::string, std::deque<std::string>> peerReader::readMap;
std::mutex peerReader::readMapMutex;

peerReader::peerReader() {}

/**
 * ���캯�� ��ʼʱ����static map����keyֵ
 */
peerReader::peerReader(SOCKET clientSocket, const std::string &ipAddress) : clientSocket(clientSocket),
                                                                            ipAddress(ipAddress) {
    std::deque<std::string> readQueue;
    readMapMutex.lock();
    if (peerReader::readMap.count(this->ipAddress) == 0) {
        peerReader::readMap[this->ipAddress] = readQueue;
    }
    readMapMutex.unlock();
}

/**
 * Monitor for incoming messages
 */
void peerReader::run() {
    char receiveMsgBuff[200];
    memset(receiveMsgBuff, 0, sizeof(receiveMsgBuff));

    // result = 0: socket disconnected
    // result < 0: error
    // result > 0: length of message received
    while (true) {
        int result = recv(clientSocket, receiveMsgBuff, sizeof(receiveMsgBuff), 0);
        if (result < 0) {
            std::cout << "peerReader run(): recv return value which < 0" << std::endl;
            break;
        } else {
            std::cout << "peerReader run(): \"" << receiveMsgBuff << "\"" << std::endl;

            std::string msg(receiveMsgBuff);

            // ȥ����β�Ļ��з� \r\n
            while (msg.back() == '\r' || msg.back() == '\n') {
                msg.pop_back();
            }
            //todo:������� 2019��8��3��15:17:23 ������
            readMapMutex.lock();
            peerReader::readMap[this->ipAddress].push_back(msg);
            readMapMutex.unlock();
        }
        //todo:��˯��
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // ��������»�һֱwhileѭ������
    std::cout << "peerReader run(): socket disconnected " << std::endl;
}

/**
 * Reads all messages from a buffered queue
 * @return a collection of all messages
 */
std::vector<std::string> peerReader::readData() {
    std::vector<std::string> buffer;

    readMapMutex.lock();
    while (!peerReader::readMap[this->ipAddress].empty()) {
        buffer.emplace_back(peerReader::readMap[this->ipAddress].front());
        peerReader::readMap[this->ipAddress].pop_front();
    }
    readMapMutex.unlock();

    return buffer;
}








