#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "Socket.h"
#include "InetAddress.h"
#include <string>

using std::string;

class Acceptor
{
public:
    Acceptor(const string &ip, unsigned short port);
    ~Acceptor();
    void ready();
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
    int accept();
    int fd() const;

private:
    Socket _listenSock;
    InetAddress _servAddr;
};

#endif
