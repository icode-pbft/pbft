//
// Created by 18716 on 2019/7/13.
//

#ifndef UNTITLED_REQUESTMSG_H
#define UNTITLED_REQUESTMSG_H

#include <iostream>

using namespace std;
class RequestMsg {
private:
    string content;
public:
    const string &getContent() const;

    void setContent(const string &content);
};


#endif //UNTITLED_REQUESTMSG_H
