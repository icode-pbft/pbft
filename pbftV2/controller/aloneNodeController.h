//
// Created by 18716 on 2019/8/1.
//

#ifndef PBFTV2_ALONENODECONTROLLER_H
#define PBFTV2_ALONENODECONTROLLER_H

#include <vector>
#include "../msg/Msg.h"
#include "../config/Config.h"
#include <mutex>
#include <map>
#include <algorithm>
#include <thread>
using namespace std;

class aloneNodeController {

private:

    ////�����ļ���ȡ
    static Config configSettings;
    ////�������
    static int nodeNo;
    ////request�����б�
    static vector<Msg*> requestReadyList;
    ////��
    static mutex mtx;
    ////�����ڵ��б�
    static vector<aloneNodeController*> nodes;
    ////ָ����ǰ�ڵ�����ж��ٸ�����ڵ�
    static const int nodeSizeMax;
    ////ͨ��request��content
    vector<string> requestList;
    ////�Ƿ�ͨ��confirm��Ϣ
    bool isConfirm= false;
    ////pp��Ϣ����
    vector<int> ppMsgList;
    ////pp��Ϣ���������
    vector<Msg*> ppMsgReadyList;
    ////��¼��Ӧ���к��յ���p��Ϣ�ĸ���
    int pTimes[6][2]={0};
    ////p��Ϣ����
    vector<int> pMsgList;
    ////��¼p��Ϣ�����ĸ�����,���������к�+��&��+������
    vector<string> pMsgNodeNos;
    ////p��Ϣ���������
    vector<Msg*> pMsgReadyList;
    ////��¼��Ӧ���к��յ���c��Ϣ�ĸ���
    int cTimes[6][2]={0};
    ////c��Ϣ����
    vector<int> cMsgList;
    ////��¼c��Ϣ�����ĸ����������������к�+��&��+������
    vector<string> cMsgNodeNos;
    ////c��Ϣ���������
    vector<Msg*> cMsgReadyList;
    ////�Ƿ�Ϊ���ڵ�
    bool isMain= false;
    ////�Ƿ�ΪͶƱ�ڵ�
    bool isVote= false;
    ///ϵͳ
    int viewNo;
    ///ϵͳ�ڵ�ı�ʶ
    int systemId;

    int highLine;

    int lowLine;

    ////���ڵ����ɵ����к�
    int serialNo=1;

    ////�ִ�
    int times=0;

    ////ͶƱ�ڵ�ĸ���
    int chooseSize;

public:

    aloneNodeController();

    ~aloneNodeController();


    /**
    * ��������
    * @param msg
    */
    static void start(Msg* msg);

    /**
    * ��ڷ���
    * @param msg
    */
    void action(Msg* msg);

    /**
    * ɾ��nodes���һ��ڵ㲢delete
    */
    static void removeHalfNodes();

    /**
     * ����confirm��Ϣ
     * @param confirmMain
     */
    void dealWithConfirm(Msg* confirmMain);

    /**
    * ����request��Ϣ
    * @param confirmMain
    */
    void dealWithRequest(Msg *request);

    /**
     * ���������request��Ϣ����
     * @param remark
     */
    void dealWithReadyRequest();

    /**
     * ����pp��Ϣ
     * @param ppMsg
     */
    void dealWithPpMsg(Msg* ppMsg);

    /**
     * ���������PP��Ϣ����
     * @param serialNo
     */
    void dealWithReadyPpMsg(int serialNo);

    void dealWithReadyPpMsg();

    /**
     * ���ppMsg�Ƿ����Ҫ��
     * @param ppMsg
     * @return
     */
    bool checkPpMsg(Msg* ppMsg);

    /**
     * ����p��Ϣ
     * @param pMsg
     */
    void dealWithPMsg(Msg* pMsg);

    /**
     * ���������P��Ϣ����
     * @param serialNo
     */
    void dealWithReadyPMsg(int serialNo);

    /**
     * �����������Ϣ����
     * @param msg
     * @param serialNo
     */
    void dealWithReadyMsg(vector<Msg*>& msgs,int serialNo);

    void dealWithReadyMsg(vector<Msg*>& msgs);

    /**
     * ���pMsg�Ƿ����Ҫ��
     * @param pMsg
     * @return
     */
    bool checkPMsg(Msg* pMsg);

    /**
     * ����c��Ϣ
     * @param commit
     */
    void dealWithCMsg(Msg* commit);

    /**
     * ���������C��Ϣ����
     * @param serialNo
     */
    void dealWithReadyCMsg(int serialNo);

    /**
     * ���cMsg�Ƿ����Ҫ��
     * @param commit
     * @return
     */
    bool checkCMsg(Msg* commit);

    /**
     * ���Msg�Ƿ����Ҫ��
     * @param ppMsg
     * @return
     */
    bool check(Msg * ppMsg);

    /**
     * �㲥msg��Ϣ
     * @param msg
     */
    void sendMsg(Msg* msg);

    /**
     * ����reply��ϵͳ
     * @param reply
     * @return
     */
    static int sendReply(Msg* reply);

    /**
     * ����hashֵ��ת��Ϊstring����
     * @param content
     * @return
     */
    static string getHashCode(string content);

    /**
     * ��Ӧ���������Ƿ��и�int����
     * @param ve
     * @param num
     * @return true : �� false: ����
     */
    static bool somethingIn(vector<int> ve,int num);

    /**
     * ��Ӧ���������Ƿ��и��ַ���
     * @param ve
     * @param str
     * @return true : �� false: ����
     */
    static bool somethingIn(vector<string> ve,string str);

    /**
     * ��Ӧ���������Ƿ��и���Ϣ
     * @param ve
     * @param object
     * @return true : �� false: ����
     */
    static bool objectIn(vector<Msg*> ve,Msg* object);

    /**
     * ��Ӧ��map���Ƿ��ж�Ӧ����ͼ��
     * @param mp
     * @param viewNo
     * @return true : �� false: ����
     */
    static bool somethingIn(map<int,bool>mp,int viewNo);

    /**
     * �鿴msgs���������޶�Ӧ�����кŵ�msg
     * @param msgs
     * @param serialNo
     * @return
     */
    static bool MsgIn(vector<Msg*> msgs,int serialNo);

    /**
     * ���ڵ��б��ж�Ӧ��ͼ�ŵĽڵ��λ�ã��������򷵻�-1
     * @param ve
     * @param viewNo
     * @return
     */
    static int nodeIn(vector<aloneNodeController*> ve,int viewNo);

    /**
     * ��msg�����Ӧ���͵Ĵ��������
     * @param node
     * @param msg
     */
    static void pushReadMsg(aloneNodeController*node, Msg* msg);

    /**
     * ��ʼ��
     * @param nodeNo
     */
    static void init(int nodeNo);

    int getViewNo() const;

    void setViewNo(int viewNo);

    int getTimes() const;

    void setTimes(int times);


    /**
     * ������
     * @param msg
     * @param nodeNo
     */
    static void getMsg(Msg* msg,int nodeNo);


    /**
     * ���
     * @param msg
     */
    static void putString(string msg);

    /**
     *
     * @param msgs
     */
    void deleteMsg(vector<Msg*> msgs);

    int countMsg(vector<string> ve,int serialNo);

};


#endif //PBFTV2_ALONENODECONTROLLER_H
