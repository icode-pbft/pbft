//
// Created by t0106 on 2019/7/14.
//

#include <fstream>
#include <thread>
#include <mutex>
#include <string>

#include "peerReader.h"
#include "../utils/commonUtils.h"
std::map<std::string ,std::queue<std::string>> peerReader::readMap;
std::mutex peerReader::readMapMutex;
peerReader::peerReader() {}

// 构造函数
peerReader::peerReader(SOCKET clientSocket) : clientSocket(clientSocket) {
    this->clientSocket = clientSocket;
}

void peerReader::run() {
    char receiveMsgBuff[200];
    memset(receiveMsgBuff, 0, sizeof(receiveMsgBuff));
    bool flag = true;

    // 返回值为0代表连接中断，<0是异常，>0是接收到的字符的个数
    while (true) {
        int result = recv(clientSocket, receiveMsgBuff, sizeof(receiveMsgBuff), 0);
        if (result < 0) {
            cout << "peerReader line 30: recv return value which < 0" << endl;
            break;
        } else {
            cout << "peerReader line 33: \"" << receiveMsgBuff << "\"" << endl;

            // ！！！: 默认每条数据只有一行！！！
            // ！！！可能会造成还没上一条存的数据还没被读出来就被下一条接收到的数据覆盖了
            std::string msg(receiveMsgBuff);

            // 去除结尾的换行符 \r\n
            while (msg.back() == '\r' || msg.back() == '\n') {
                msg.pop_back();
            }
            //todo:存入队列 2019年8月3日15:17:23 田泽鑫
            readMapMutex.lock();
            peerReader::readMap[this->fileName].push(msg);
            readMapMutex.unlock();
            //原文件存取
//            commonUtils::writeToFile(fileName, msg, std::ios::out | std::ios::trunc);
        }
        //todo:沉睡？
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }


    // 正常情况下会一直while循环阻塞
    std::cout << "peerReader line 49: socket disconnected " << std::endl;
}

//todo:读取文件改读取队列第一个
vector<string> peerReader::readData() {
    std::vector<std::string> buffer;
    std::string msg;
    //
    readMapMutex.lock();
    if (!peerReader::readMap[this->fileName].empty()) {
//        std::lock_guard<std::mutex> lck(readMapMutex);
        msg=peerReader::readMap[this->fileName].front();
        peerReader::readMap[this->fileName].pop();
    }
    readMapMutex.unlock();
//    std::string msg= peerReader::readMap[this->fileName].front();
    //原文件读取
//    commonUtils::readLineFile(fileName, msg);
    if (!msg.empty()) {
//        std::cout << "peerReader line 67 readData() msg: \"" << msg << "\"" << std::endl;
        buffer.emplace_back(msg);
        // 原文件清空内容
//        std::ofstream fileout(fileName, std::ios::out | std::ios::trunc);
//        fileout.close();

    }
    return buffer;
}


/**
 *构造函数 初始时即给static map赋予key值
 */
peerReader::peerReader(SOCKET clientSocket, const string &fileName) : clientSocket(clientSocket),
                                                                     fileName(fileName) {
            std::queue<string> readQueue;
            peerReader::readMap[this->fileName]=readQueue;
}




