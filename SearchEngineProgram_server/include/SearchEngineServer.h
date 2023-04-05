#ifndef __SEARCHENGINESSERVER_H__
#define __SEARCHENGINESSERVER_H__

#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "JsonTrans.h"
#include "KeyRecommander.h"
#include "queryPageWeb.h"
#include "log4cpp.hpp"

#define PAGEPWD "../conf/page/" //web模块用

using std::cout;
using std::endl;

//static myLog &log = myLog::init();//初始化一个log单例出来
//
class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg)
    , _con(con)
    ,_keyRecommander("init")
    ,_webPageSearcher(PAGEPWD)
    {}
    //该函数在线程池中执行的（计算线程），已经通过bind打包给线程池的线程了
    //IO线程和计算线程通信部分
    //计算线程发送给IO线程执行send命令
    //send发送给客户端
    void process()
    {
        //在process函数中去进行真正的业务逻辑的处理
        /* string newMsg = msg + 1; */
        //现在计算线程（ThreadPool）处理完业务逻辑之后，需要
        //将处理完后数据发送给IO线程（EventLoop/Reactor），
        //IO线程如何与计算线程进行通信？(eventfd)
        //static myLog &log = myLog::init();//初始化一个log单例出来
        //cout<<"请输入类型：1.关键字推荐 2.网页库查询"<<endl;
        //拆分
        char c_num[100]="";//存"1."
        char c_real_msg[100]="";//存"hello"

        char c_msg[1024]="";//存"1.hello"
        strcpy(c_msg,_msg.c_str());//将_msg转成c串

        int i=0;
        while(c_msg[i]!='.')
        {
            c_num[i]=c_msg[i];
            i++;
        }
        i=i+1;
        int j=0;
        while(c_msg[i]!='\n')
        {
            c_real_msg[j]=c_msg[i];
            i++;
            j++;
        }
        string num(c_num);//数字"1."
        string real_msg(c_real_msg);//真正内容如："hello"
        
        string k_w_n_Msg;//实际传输 key or web or nothing
        
        if (num.compare("1") == 0)
        {
            logInfo("关键词推荐：%s",c_real_msg);
            k_w_n_Msg=(_keyRecommander.doQuery(real_msg));
            
        }
        else if (num.compare("2") == 0)
        {

            logInfo("网页查询：%s",c_real_msg);
            vector<queryResult> qrs;
            _webPageSearcher.query(real_msg,qrs); //结果全部存在qrs中（vector）
            for (auto &elem : qrs)
            {
                k_w_n_Msg+="\n";
                k_w_n_Msg+="标题：";k_w_n_Msg+=elem.title;k_w_n_Msg+="\n";
                k_w_n_Msg+="摘要：";k_w_n_Msg+=elem.description;k_w_n_Msg+="\n";
                k_w_n_Msg+="链接：";k_w_n_Msg+=elem.link;k_w_n_Msg+="\n";
            }
        }
        else
        {
            k_w_n_Msg = "请选择正确选项\n";
            logDebug("请选择正确选项：%s",c_real_msg);
        }
        
        _con->sendInLoop(k_w_n_Msg);
        cout<<"查询完毕"<<endl;
        //线程池处理完业务逻辑之后，将k_w_n_Msg给eventloop
        //IO线程和计算线程的分离
        //数据的发送需要在EventLoop里面进行发送
        //TcpConnection需要将数据发送给EventLoop，让EventLoop去
        //进行发送数据IO操作
        //此时TcpConnection需要知道EventLoop存在
        //想要知道EventLoop的存在同时又弱耦合，即在TcpConnection中创建指向EventLoop的指针
    }
private:
    string _msg;
    TcpConnectionPtr _con;
    KeyRecommander _keyRecommander; //关键字查询
    querypageWeb _webPageSearcher; //网页库查询
};

class SearchEngineServer
{
public:
    SearchEngineServer(size_t threadNum, size_t queSize
               , const string &ip, unsigned short port)//线程数、队列大小、ip、port
    : _threadpool(threadNum, queSize)
    , _tcpserver(ip, port)
    {

    }

    void start()
    {
        _threadpool.start();//计算线程全部启动
        using namespace std::placeholders;
        _tcpserver.setAllCallback(bind(&SearchEngineServer::onConnection, this, _1)
                               , bind(&SearchEngineServer::onMessage, this, _1)
                               , bind(&SearchEngineServer::onClose, this, _1));
        _tcpserver.start();//IO线程启动
    }

    void stop()
    {
        _threadpool.stop();
        _tcpserver.stop();
    }

    void onConnection(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has connected!" << endl;
        //cout<<"请输入类型: 1.关键字推荐 2.网页库查询"<<endl;
    }
    
    void onMessage(const TcpConnectionPtr &con)
    {
        json recvjMsg = json::parse(con->receive().c_str());
        Data dMsg;
        bzero(&dMsg,sizeof(dMsg));
        jsonTrans jTrans(dMsg);//dMsg->jTrans._data转换
        jTrans.from_json(recvjMsg);//处理过的json包，数据在jTrans._data
        //jTrans.printData();
        //收到"1.hello"
        //处理msg的业务逻辑全部在此处实现的话
        //将msg这些信息打个包交给MyTask，然后将MyTask注册给线程池
        //让线程池去处理具体的业务逻辑，此时业务逻辑的处理就不在
        //EventLoop线程中
        cout<<"查询中..."<<endl;
        MyTask task1(jTrans.getDatass(), con);//将json的string格式（未解析串）分配给task//mytask初始化列表中连接key、web
        //_threadpool.addTask(std::bind(&MyTask::process, task));//将mytask类task对象中的process函数打包放进线程池的任务队列，准备实行
        _threadpool.addTask(std::bind(&MyTask::process, task1));

    }
    
    void onClose(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has closed!" << endl;
    }
private:
    ThreadPool _threadpool;
    TcpServer _tcpserver;
    
    //ProtolParser _protocolParser; //协议类解析
};

#endif
