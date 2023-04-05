#include "../include/TcpConnection.h"
#include "../include/EventLoop.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
: _loop(loop)
, _sock(fd)
, _sockIO(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
//,_type(0)

{

}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::send(const string &msg)
{
    //string mmssgg;
    //cout<<"真实内容："<<msg<<"1"<<endl;
    //cout<<"大小："<<sizeof(msg)<<endl;
    //if(msg.compare("1\n")==0)
    //{
    //    mmssgg="关键词推荐\n";
    //    _type=1;
    //}
    //else if(msg.compare("2\n")==0)
    //{
    //    mmssgg="网页查询\n";
    //    _type=2;
    //}
    //else
    //{
    //    mmssgg="请选择正确选项\n"; 
    //    _type=3;  
    //}
    //string mmssgg="你做出了选择\n";
    //_sockIO.writen(mmssgg.c_str(), mmssgg.size());

    //_sockIO.writen(msg.c_str(), msg.size());

    Data dMsg;
    dMsg.length=msg.size();
    dMsg.ss=msg;
    jsonTrans jTrans(dMsg);

    json jMsg;
    jTrans.to_json(jMsg);//jTrans._data->jMsg
    _sockIO.writen(jMsg.dump().c_str(),jMsg.dump().size());

}

void TcpConnection::sendInLoop(const string &msg)
{
    if(_loop)
    {
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));//注册到EventLoop
        //void runInLoop(function<void()> &&cb);
    }
}

string TcpConnection::receive()
{
    char buff[65535] = {0};
    //cout<<"tcp::a"<<endl;
    //_sockIO.readLine(buff, sizeof(buff));
    _sockIO.recvs(buff);
    //cout<<"tcp::b"<<endl;
    return string(buff);
}

string TcpConnection::toString()
{
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();
    
    return oss.str();
}

bool TcpConnection::isClosed() const
{
    char buf[10] = {};
    int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);

    return (ret == 0);
}

//获取本端的网络地址信息
InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname");
    }

    return InetAddress(addr);

}

//获取对端的网络地址信息
InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getpeername");
    }

    return InetAddress(addr);

}

void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb)
{
    _onConnectionCb = std::move(cb);
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{
    _onMessageCb = std::move(cb);
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{
    _onCloseCb = std::move(cb);
}

//三个事件回调函数的执行
void TcpConnection::handleConnectionCallback()
{
    if(_onConnectionCb)
    {
        /* using TcpConnectionPtr = shared_ptr<TcpConnection>; */
        /* using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>; */
        /* TcpConnectionCallback _onConnectionCb; */
        //function<void(const shared_ptr<TcpConnection> &)> _onConnectionCb;
        _onConnectionCb(shared_from_this());
    }
}
void TcpConnection::handleMessageCallback()
{
    if(_onMessageCb)
    {
        _onMessageCb(shared_from_this());
    }

}
void TcpConnection::handleCloseCallback()
{
    if(_onCloseCb)
    {
        _onCloseCb(shared_from_this());
    }

}
