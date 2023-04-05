#ifndef __CLIENTCONNECT_H__
#define __CLIENTCONNECT_H__

#include "Socket.h"
#include "InetAddress.h"
#include "SocketIO.h"
#include<string>

using std::string;

class ClientConnect
{
public:
    ClientConnect(const std::string &ip, unsigned short port)
    : _sockio(_sock.fd()), _serverAddr(ip, port)
    {
        connect();
    }
    int getSockfd() const { return _sock.fd(); }
    json recvMessage()  { return _sockio.recvMessage();}
    void sendMessage(jsonTrans &jTrans)  { _sockio.sendMessage(jTrans);}

private:
    void connect() const
    {
        int ret = ::connect(_sock.fd(), (sockaddr *)&_serverAddr.addr(), sizeof(_serverAddr.addr()));
    }

public:
    Socket _sock;
    SocketIO _sockio;
    InetAddress _serverAddr;
};
#endif