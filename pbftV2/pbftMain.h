//
// Created by t0106 on 2019/8/1.
//

#ifndef PBFTV2_PBFTMAIN_H
#define PBFTV2_PBFTMAIN_H


#include "controller/aloneNodeController.h"
#include "p2p/peerNetwork.h"

class pbftMain {
public:
    static aloneNodeController nodeController;

    static peerNetwork network;

public:
    static void broadcast(std::string sendMsg);

    static void sendToNode(std::string sendMsg, int nodeNo);
};


#endif //PBFTV2_PBFTMAIN_H
