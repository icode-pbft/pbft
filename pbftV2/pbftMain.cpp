//
// Created by t0106 on 2019/8/1.
//
#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <thread>
#include <vector>

#include "pbftMain.h"

#include "p2p/peerNetwork.h"
#include "controller/aloneNodeController.h"
#include "utils/commonUtils.h"

// ???8015?????????
int port = 8015;
std::string ipPrefix = "10.25.15.";
aloneNodeController pbftMain::nodeController;
peerNetwork pbftMain::network(8015);

void initWinsock() {
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        std::cout << "Failed to load WinSock" << std::endl;
        std::cout << "????WinSock???" << std::endl;
    }
    std::cout << "????WinSock???" << std::endl;


    std::cout << "Starting peer network... " << std::endl;
}

int main() {
    // todo: ?????��??
    aloneNodeController::setNodeNo(1);

    initWinsock();

    // ????????????????????peerThreads??��
    std::thread networkThread(&peerNetwork::run, std::ref(pbftMain::network));
    networkThread.detach();
    std::cout << "[  Node is started in port: " << port << "  ]" << std::endl;

    std::vector<std::string> peers = std::vector<std::string>(0);
    // ??peers.list????????????ip&port��??????
    if (access("../config/peers.list", 0) < 0) {
        char ip[16] = {0};
        if (commonUtils::getLocalIP(ip) == 0) {
            std::string msg = string(ip) + ":" + std::to_string(port);
            commonUtils::writeToFile("../config/peers.list", msg, std::ios::out);
        }
    } else {
        ifstream inFile;
        inFile.open("../config/peers.list", std::ios::in);
        string str;
        while (inFile.peek() != EOF) {
            std::getline(inFile, str);
            peers.emplace_back(str);

            vector<string> sv;
            commonUtils::split(str, sv, ":");

            std::cout << "Node line 65: " << sv[0] << ":" << sv[1] << std::endl;
            // ??????????
            if (commonUtils::isLocal(sv[0]) && std::to_string(port) == sv[1]) {
                continue;
            }

            pbftMain::network.connectToPeer(sv[0], std::stoi(sv[1]));
        }

        inFile.close();
    }

    bool runFlag = true;
    while (runFlag) {
        for (const auto &peer : pbftMain::network.peers) {
            if (std::find(peers.begin(), peers.end(), peer) == peers.end()) {
                std::cout << "-" << peer << "-" << std::endl;
                peers.emplace_back(peer);
                commonUtils::writeToFile("peers.list", peer, std::ios::app);
            }
        }
        // peerNetwork1.peers??????????
        pbftMain::network.peers.clear();

        for (auto pt : pbftMain::network.peerThreads) {
//            std::cout << "-" << pbftMain::network.peerThreads.size() << "-" << std::endl;
            std::vector<std::string> dataVector = pt.peerReader1.readData();

            for (std::string data : dataVector) {
                // todo: ??????????
                cout << "[p2p] COMMAND:: " << data << endl;

                // todo: data?????��??Msg????
                // todo: ????????????Msg??????
                Msg *msg = Msg::fromJson(data);

//                std::thread t1(&aloneNodeController::start, &msg);
//                t1.detach();
                pbftMain::nodeController.start(msg);
            }
        }

         // ?30ms
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}


/**
 * ??
 */
void pbftMain::broadcast(std::string sendMsg) {
    pbftMain::network.broadcast(sendMsg);
}

/**
 * ???????????
 * @param sendMsg
 * @param nodeNo
 */
void pbftMain::sendToNode(std::string sendMsg, int nodeNo) {
    for (auto pt : pbftMain::network.peerThreads) {
        if (ipPrefix+std::to_string(nodeNo) == pt.ipAddress) {
            pt.send(sendMsg);
        }
    }
}
