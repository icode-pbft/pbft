//
// Created by 18716 on 2019/7/13.
//

#include "RequestMsg.h"

const string &RequestMsg::getContent() const {
    return content;
}

void RequestMsg::setContent(const string &content) {
    RequestMsg::content = content;
}
