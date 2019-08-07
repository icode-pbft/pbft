//
// Created by t0106 on 2019/7/25.
//
#include <WinSock2.h>
#include <winsock.h>
#include <cstring>
#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <string>

#include "commonUtils.h"

int commonUtils::getLocalIP(char *ip) {
    char host[100] = {"sina.com.cn"};
    if (gethostname(host, sizeof(host)) < 0)
        return -1;

    struct hostent *hp;
    if ((hp = gethostbyname(host)) == nullptr)
        return -1;
    strcpy(ip, inet_ntoa(*(struct in_addr *) hp->h_addr));   //h_addr代表主机IP
    return 0;
}

void commonUtils::split(const std::string s, std::vector<std::string> &sv, const char *delimit) {
    sv.clear();

    char *p = std::strtok(const_cast<char *>(s.data()), delimit);
    do {
        sv.emplace_back(p);
    } while ((p = std::strtok(nullptr, delimit)));
}

bool commonUtils::isLocal(std::string host) {
    char hostname[30];
    gethostname(hostname, sizeof(hostname));

    struct hostent *hp;
    if ((hp = gethostbyname(hostname)) == nullptr) {
        std::cout << "commonUtils::isLocal gethostbyname error " << std::endl;
    }

    for (int i = 0; hp->h_addr_list[i] != nullptr; i++) {
        if (host == inet_ntoa(*(struct in_addr *) hp->h_addr_list[i])) {
            return true;
        }
    }

    return false;
}

std::string commonUtils::sha256(const std::string &str) {
    char buf[1024];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::string NewString;
    for (unsigned char i : hash) {
        sprintf_s(buf, "%02x", i);
        NewString += buf;
    }

    return NewString;
}

void commonUtils::writeToFile(const std::string &fileName, const std::string &msg, std::ios_base::openmode operate) {
    std::ofstream outFile;

    std::cout << "****************************************" << std::endl;
    std::cout << "Ready to write to " + fileName + ": \"" + msg << "\"" << std::endl;
    outFile.open(fileName, operate);
    outFile << msg << std::endl;
    outFile.close();
    std::cout << "Write " + fileName + " complete" << std::endl;
}

void commonUtils::readLineFile(const std::string &fileName, std::string &msg) {
    std::ifstream inFile;
    inFile.open(fileName, std::ios::in);
    std::string str;
    if (inFile.peek() != EOF) {
        // 是否读取到内容应由调用方确认
        std::getline(inFile, msg);
    }

    inFile.close();
}

// 待用
size_t commonUtils::hashCode(const std::string &str) {
    std::hash<std::string> hashTest;
    return hashTest(str);
}
