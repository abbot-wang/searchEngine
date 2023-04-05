#include "../include/EventLoop.h"
#include <unistd.h>
#include <sys/eventfd.h>

EventLoop::EventLoop(Acceptor &acceptor)
: _epfd(createEpollFd())
, _evtfd(createEventFd())
, _acceptor(acceptor) 
, _isLooping(false)
, _evtList(1024)
{
    addEpollReadFd(acceptor.fd());//把listenfd放在红黑树上进行监听
    addEpollReadFd(_evtfd);//放在红黑树上进行监听（evenfd：threadpool线程和eventloop线程的通信）
}

EventLoop::~EventLoop()
{
    if(_epfd)
    {
        close(_epfd);
    }

    if(_evtfd)
    {
        close(_evtfd);
    }
}

void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

void EventLoop::runInLoop(Functor &&cb)
{
    //可以使用大括号将某些栈变量/栈对象提前结束
    {
        MutexLockGuard autoLock(_mutex);
        _pengingsCb.push_back(std::move(cb));
    }

    //....
    //...
    wakeup();
}

void EventLoop::doPengingFunctors()  
{
    vector<Functor> tmp;//将旧vector通过swap导入新vector //这样就能在取旧vector内容的同时又能push_back
    {
        //大括号使得粒度更小（粒度指的是mutexlock的作用范围）
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pengingsCb);
    }

    //vector<Functor> _pengingsCb;
    for(auto &cb : tmp)
    {
        cb();
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = ::write(_evtfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("write");
        return;
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    int ret = ::read(_evtfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("read");
        return;
    }
}
void EventLoop::waitEpollFd()
{
    int nready = -1;
    do
    {
        nready = ::epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 5000);
    }while(-1 == nready && errno == EINTR);

    if(-1 == nready)
    {
        perror("nready == -1");
        return;
    }
    else if(0 == nready)
    {
        printf(">>epoll_wait timeout\n");
    }
    else
    {
        if(nready == (int)_evtList.size())
        {
            _evtList.resize(2 * nready);//考虑到了监听超过上限的问题
        }

        for(int idx = 0; idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;
            if(fd == _acceptor.fd())//有新的连接请求进来
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleNewConnection();
                }
            }
            else if(fd == _evtfd)
            {
                handleRead();
                doPengingFunctors();
            }
            else
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleMessage(fd);//有数据的传输
                }

            }
        }
    }
}
void EventLoop::handleNewConnection()
{
    //只要peerfd有正确的返回值，就证明三次握手已经建立完成
    int peerfd = _acceptor.accept();
    addEpollReadFd(peerfd);//把peerfd放到红黑树上进行监听

    TcpConnectionPtr con(new TcpConnection(peerfd, this));

    //Tcp连接创建之后就可以进行三个事件的注册
    con->setConnectionCallback(_onConnectionCb);//连接建立
    con->setMessageCallback(_onMessageCb);//消息的到达
    con->setCloseCallback(_onCloseCb);//连接的断开

    _conns.insert(std::make_pair(peerfd, con));
    con->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    auto iter = _conns.find(fd);
    if(iter != _conns.end())
    {
        //用Tcp连接里面定义一个函数
        //isClose,里面执行recv，recv的返回值等于0
        //
        bool flag = iter->second->isClosed();
        if(flag)
        {
            //连接是断开的
            iter->second->handleCloseCallback();
            delEpollReadFd(fd);//将文件描述符从红黑树上删除
            _conns.erase(iter);//将文件描述符从map中删除
        
        }
        else
        {
            //连接是正常
            iter->second->handleMessageCallback();
        }

    }
    else
    {
        /* cout << "该连接不存在" << endl; */
        printf("该连接不存在\n");
    }
}

int EventLoop::createEpollFd()
{
    int fd = epoll_create(100);
    if(-1 == fd)
    {
        perror("epoll_create");
        return -1;
    }
    return fd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if(ret == -1)
    {
        perror("epoll_ctl add");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret == -1)
    {
        perror("epoll_ctl del");
        return;
    }
}

void EventLoop::setConnectionCallback(TcpConnectionCallback &&cb)
{
    _onConnectionCb = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
    _onMessageCb = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{
    _onCloseCb = std::move(cb);
}
int EventLoop::createEventFd()
{
    int fd= eventfd(10, 0);
    if(-1 == fd)
    {
        perror("eventfd");
        return -1;
    }

    return fd;
}
