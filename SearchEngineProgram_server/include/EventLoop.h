#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "MutexLock.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

using std::vector;
using std::map;
using std::shared_ptr;
using std::function;


using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
using Functor = function<void()>;//runInLoop

class EventLoop
{
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();
    void loop();
    void unloop();

    void runInLoop(Functor &&cb);//function<void()> //将发送往任务端的send依次存入vector

    void wakeup();//将线程唤醒
    void handleRead();

    void doPengingFunctors();//从vector中将待执行的任务依次执行（任务来自于threalpool处理好的业务逻辑，准备依次发往客户端）

    void setConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    int createEpollFd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);
    int createEventFd();

private:
    int _epfd;//红黑树的根节点
    int _evtfd;//IO线程和计算线程之间的通信
    Acceptor &_acceptor;
    bool _isLooping;
    vector<struct epoll_event> _evtList;
    /* map<int, shared_ptr<TcpConnection>> _conns; */
    map<int, TcpConnectionPtr> _conns;
    
    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

    vector<Functor> _pengingsCb;//子线程传来的IO任务排队，依次发给客户端
    MutexLock _mutex;//初始化列表中未写，则自动调用MutexLock的无参构造函数
};

#endif
