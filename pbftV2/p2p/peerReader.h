//
// Created by t0106 on 2019/7/14.
//

#ifndef TCP_TEST_PEERREADER_H
#define TCP_TEST_PEERREADER_H

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

class peerReader {
private:
    SOCKET clientSocket;
    //
    std::string fileName;
public:
    peerReader(SOCKET clientSocket, const std::map<int, std::queue<string>> &readMap);
    /**
     * 2019年8月3日09:59:00 田泽鑫 文件读写改为map<>存取key为IP,value为一个队列，存取要读的消息
     * */
    static std::map<std::string, std::queue<std::string>> readMap;

    static std::mutex readMapMutex;
private:

    vector<string> receivedData = vector<string>(0);

public:
    peerReader();

    peerReader(SOCKET clientSocket);

    peerReader(SOCKET clientSocket, const string &fileName);

    void run();

    // 取出缓冲数据
    vector<string> readData();
};


#endif //TCP_TEST_PEERREADER_H
