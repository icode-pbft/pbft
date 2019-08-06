//
// Created by t0106 on 2019/7/25.
//

#ifndef TCP_TEST_COMMONUTILS_H
#define TCP_TEST_COMMONUTILS_H

#include <iostream>
#include <vector>

class commonUtils {
public:
    static int getLocalIP(char *ip);

    static void split(const std::string s,
                      std::vector<std::string> &sv,
                      const char *delimit = " ");

    static bool isLocal(std::string host);

    static std::string sha256(const std::string &str);

    static void writeToFile(const std::string &fileName, const std::string &msg, std::ios_base::openmode operate);

    static void readLineFile(const std::string &fileName, std::string &msg);

    static size_t hashCode(const std::string &str);
};


#endif //TCP_TEST_COMMONUTILS_H
