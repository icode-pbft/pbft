//
// Created by 18716 on 2019/8/1.
//

#ifndef PBFTV2_ALONENODECONTROLLER_H
#define PBFTV2_ALONENODECONTROLLER_H

#include <vector>
#include <mutex>
#include <algorithm>
#include <thread>

#include "../msg/Msg.h"
#include "../config/Config.h"

using namespace std;
class aloneNodeController {

private:
    /// request��Ϣ���������
    static vector<Msg*> requestReadyList;
    static vector<aloneNodeController*> nodes;
    /// nodes������Žڵ�ĸ���
    static const int nodeSizeMax;
    /// �ڵ���
    static int nodeNo;
    /// ͨ����pp��Ϣ
    vector<Msg*> ppMsgs;
    /// pp��Ϣ����
    vector<int> ppMsgList;
    /// pp��Ϣ���������
    vector<Msg*> ppMsgReadyList;
    /// ��¼��Ӧ���к��յ���p��Ϣ�ĸ���
    int pTimes[6][2];
    /// ͨ����p��Ϣ����
    vector<int> pMsgList;
    /// p��Ϣ���������
    vector<Msg*> pMsgReadyList;
    /// ��¼��Ӧ���к��յ���c��Ϣ�ĸ���
    int cTimes[6][2];
    /// ͨ����c��Ϣ����
    vector<int> cMsgList;
    /// c��Ϣ���������
    vector<Msg*> cMsgReadyList;
    /// �Ƿ�ΪͶƱ�ڵ�
    bool isVote;
    /// ��ͼ���
    int viewNo;
    /// ��ˮλ��
    int highLine;
    /// ��ˮλ��
    int lowLine;
    /// ���к�
    int serialNo;
    /// �ִ�
    int times;
    /// ͶƱ�ڵ�ĸ���
    int chooseSize;
    /// ���ڶ�ȡ�����ļ�
    Config configSettings;
    /// ϵͳip
    int systemIp;
public:
public:
    /**
     * ��ʶ��ʼ
     * @param msg
     */
    static void start(Msg* msg);

    /**
     * ɾ��nodes���һ��ڵ㲢delete
     */
    static void removeHalfNodes();

    /**
     * �յ���Ϣ�Լ���Ӧ����
     * @param msg
     */
    void action(Msg* msg);

    /**
     * ����Confirm��Ϣ
     * @param confirmMain
     */
    void dealWithConfirm(Msg* confirmMain);

    /**
     * ����Request��Ϣ
     * @param request
     */
    void dealWithRequest(Msg* request);

    /**
     * ����pp��Ϣ
     * @param ppMsg
     */
    void dealWithPpMsg(Msg* ppMsg);

    /**
     * ����p��Ϣ
     * @param pMsg
     */
    void dealWithPMsg(Msg* pMsg);

    /**
     * ����c��Ϣ
     * @param commit
     */
    void dealWithCMsg(Msg* commit);

    /**
     * pp�׶μ���յ�����Ϣ�����к��Լ���ͼ���Ƿ���ϱ�׼
     * @param ppMsg
     * @return ����Ϊtrue
     */
    bool checkPpMsg(Msg* ppMsg);

    /**
     * p�׶���Ϣ����յ�����Ϣ��ժҪ����ͼ�š����к��Ƿ���ϱ�׼
     * @param pMsg
     * @return
     */
    bool checkPMsg(Msg* pMsg);


    /**
     * C�׶���Ϣ����յ�����Ϣ��ժҪ����ͼ�š����к��Ƿ���ϱ�׼
     * @param commit
     * @return
     */
    bool checkCMsg(Msg* commit);

    /**
     * �����Ϣ
     * @param ppMsg
     * @return
     */
    bool check(Msg * ppMsg);

    /**
     * �ڵ㷢��Ϣ
     * @param msg
     */
    static void sendMsg(Msg* msg);

    /**
     * �ڵ���ϵͳ����reply��Ϣ
     * @param reply
     */
    static void sendReply(Msg* reply);

    /**
     * ����ַ�����hashֵ
     * @param content
     * @return ��ϣֵ��ʮ�����޷���ֵ
     */
    static size_t getHashCode(string content);

    /**
     * ��ȡ�����ļ���ʼ������
     */
    void initConfig();

    /**
     * ��Ӧ���������Ƿ��и�int����
     * @param ve
     * @param num
     * @return
     */
    static bool somethingIn(vector<int> ve,int num);

    /**
     * ���ڵ��б��ж�Ӧ��ͼ�ŵĽڵ��λ�ã��������򷵻�-1
     * @param ve
     * @param viewNo
     * @return
     */
    static int nodeIn(vector<aloneNodeController*> ve,int viewNo);

    /**
     * д��־
     * @param message
     */
    static void writeLog(string message);

    /// getter��setter����
    static void setNodeNo(int nodeNo);

    int getViewNo() const;

    void setViewNo(int viewNo);

    int getSystemIp() const;

};


#endif //PBFTV2_ALONENODECONTROLLER_H
