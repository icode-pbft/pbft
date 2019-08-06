//
// Created by t0106 on 2019/7/14.
//
#include <thread>
#include <functional>

#include "peerThread.h"

using std::thread;

/**
 * p2pͨѶ�߳�
 * �ڽ����׽��ֺ󣬷ֳ������������̣߳�һ�������������ݣ�һ������������ݣ���˵������ݲ�������
 *
 * @author Mignet
 */
// C++�����?
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

    // ����ŵúܣ���ȫ��֪���ܲ���������
//    thread peerReaderThread(&peerReader::run, std::ref(peerReader1));
    thread peerReaderThread(&peerReader::run, peerReader1);
    peerReaderThread.detach();

//    thread peerWriterThread(&peerWriter::run, std::ref(peerWriter1));
    thread peerWriterThread(&peerWriter::run, peerWriter1);
    peerWriterThread.detach();
}

void peerThread::send(string data) {
    // ��Java��Ҫ��peerWrite1�����пգ���C++��һ��������ָ���ָ�룬����
    // Clang-Tidy: Parameter 'data' is passed by value and only copied once;
    // consider moving it to avoid unnecessary copies
    // �������Ҹ�������warning��C++??
    peerWriter1.write(data);
}



peerThread::peerThread() {}

