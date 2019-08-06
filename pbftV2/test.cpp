//
// Created by 18716 on 2019/7/30.
//

//using namespace std;
//
//class Teacher1;
//
//class Student{
//public:
//    void show(Teacher1 teacher1);
//};
//
//class Teacher1{
//private:
//    string m_name;
//    int m_age;
//public:
//    Teacher1(string name,int age):m_name(name),m_age(age){
//    };
//    friend class Student;
//};
//
//
//void Student::show(Teacher1 teacher1){
//    cout<<teacher1.m_age<<"        "<<teacher1.m_name<<"***"<<teacher1.m_name.length();
//};


//#include "pbft/controller/NodeController.h"
//bool flag[2];
//int turn=0;
//
//
//void* test3(void* args){
//    NodeController * nodeController=(NodeController*)args;
//    for (int i = 0; i < 20; ++i) {
//        cout<<nodeController->getState()<<endl;
//    }
//    return 0;
//}
//
//void* test1(void* args){
//
//
//    NodeController* nodeController=(NodeController*)args;
//    for (int i = 0; i < 20; ++i) {
////        cout<<"进程1"<<endl;
//        flag[0]= true,turn=1;
//        while (flag[1]&&turn==1);
//        nodeController->setState(nodeController->getState()+1);
//        std::cout<<nodeController->getState()<<endl;
//
//
//
//        flag[0]= false;
//    }
//
//
//    return 0;
//}
//void* test2(void* args){
//
//
//    NodeController* nodeController=(NodeController*)args;
//    for (int i = 0; i < 20; ++i) {
////        cout<<"进程2"<<endl;
//        flag[1]= true;
//        turn=0;
//        while (flag[0]&&turn==0);
//        nodeController->setState(nodeController->getState()+1);
//        std::cout<<nodeController->getState()<<endl;
//
//
//        flag[1]= false;
//    }
//
//
//    return 0;
//}
//
//// 线程的运行函数
//void* say_hello(void* args)
//{
//    srand((int)time(0));
//    printf("%d\n",rand());
//    return 0;
//}


//int main(){
//    flag[1]= false;
//    flag[0]= true;
//    pthread_t tids[2];
//    NodeController* nodeController=new NodeController();
//
//    int ret1 = pthread_create(&tids[0], NULL, test1, nodeController);
//    int ret2 = pthread_create(&tids[1], NULL, test2, nodeController);
////    int ret1 = pthread_create(&tids[0], NULL, test3, nodeController);
////    int ret2 = pthread_create(&tids[1], NULL, test3, nodeController);
//
//    pthread_exit(NULL);
//}
//线程的基础测试
//int main()
//{
//    // 定义线程的 id 变量，多个变量使用数组
//    pthread_t tids[NUM_THREADS];
//    for(int i = 0; i < NUM_THREADS; ++i)
//    {
//        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
//        int ret = pthread_create(&tids[i], NULL, say_hello, NULL);
//        if (ret != 0)
//        {
//            cout << "pthread_create error: error_code=" << ret << endl;
//        }
//    }
//    //等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
//    pthread_exit(NULL);
//}

//int main(){
////    class Teacher test1;
////    test1.setName("刘明朗");
////    std::cout<<test1.who()<<std::endl;
////    Teacher *test2=new Teacher();
////    test2->setName("asndka");
////    std::cout<<test2->who()<<std::endl;
////    delete test2;
//Teacher1 teacher1("lml",66);
//Student student;
//student.show(teacher1);
//
//}


//随机数验证
//#include "pbft/controller/SystemController.h"
//#include "pbft/controller/NodeController.h"
//int main(){
//    srand((int)time(0));
//    set<int> choosed;
//    int time=0;
//    while(choosed.size()!=20){
//        time++;
//        int num=rand()%100;
//        cout<<"choosedSize:"<<choosed.size()<<"   num:"<<num<<endl;
//        choosed.insert(num);
//    }
//    std::vector <int> test(20);//指明大小
//
//    std::copy(choosed.begin(), choosed.end(), test.begin());
//    vector<int> test2=test;
//    for(int i:test2){
//        cout<<i<<" *";
//    }
//    std::cout <<"test 的大小"<< test.size() << std::endl;
//
//    cout<<time;
//}

//#include "pbft/msg/Msg.h"
//hash<Msg*> hashTest;
//hash<string> hashString;
//Msg* test1();
//int main(){
//    Msg* msg=test1();
//    Msg tt();
//    PpMsg* ppMsg= (PpMsg*)msg;
//    ppMsg->setType("asdasdas");
//    cout<<ppMsg->getRemark()<<endl;
//
//    size_t t1=hashString("asdasdasdas");
//    size_t t2=hashString("asdasdasdas1");
//    cout<<"t1: "<<t1<<" t2:"<<t2<<"  t1=t2?"<<(t1==t2)<<endl;
//    cout<<"****"<<msg<<endl;
//    cout<<"****"<<hashTest(msg)<<endl;
//   ;
//}
//
//Msg* test1(){
//    Msg *msg=new PpMsg();
//    cout<<"****"<<hashTest(msg)<<endl;
//    return msg;
//}
//

//测试读取配置文件

//int main(){
////    RespondMain* respondMain=new RespondMain;
////    cout<<respondMain->getType();
////    return 0;
//    int port;
//    std::string ipAddress;
//    std::string username;
//    std::string password;
//    const char ConfigFile[]= "C:\\Users\\18716\\CLionProjects\\untitled\\lml\\config.txt";
//    Config configSettings(ConfigFile);
//
//    port = configSettings.Read("port", 0);
//    ipAddress = configSettings.Read("ipAddress", ipAddress);
//    username = configSettings.Read("username", username);
//    password = configSettings.Read("password", password);
//    std::cout<<"port:"<<port<<std::endl;
//    std::cout<<"ipAddress:"<<ipAddress<<std::endl;
//    std::cout<<"username:"<<username<<std::endl;
//    std::cout<<"password:"<<password<<std::endl;
//
//    return 0;
//}




////日志测试
//#include <iostream>
//#include "pbft/log/Logger.h"
//using namespace std;
//using namespace FrameWork;
//int main(int argc,char* argv[]) {
//    InitLogging(argv[0],INFO,"./log/test");
//    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
//    LOG(INFO)<<"info test";
//    LOG(WARN)<<"WARN TEST %d"<<20;
//    LOG(ERROR)<<"Error test %d %s"<<20<<"nihao";
//    Logger::GetInstance().Error("error test common");
//    Logger::GetInstance().Fatal("fatal test common %d ",100);
//    Logger::GetInstance().Info("info test normal %d %s ",50,"zhongguoren");
//    Logger::GetInstance().Info("lml");
//    return 0;
//}