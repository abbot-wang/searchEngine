#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__
#include <arpa/inet.h>
#include <string>
class InetAddress
{
public:
    InetAddress(const std::string &ip, unsigned short port);
    InetAddress(const sockaddr_in &addr);
    std::string ip() const;
    unsigned short port() const;
    const sockaddr_in &addr() const;
private:
    sockaddr_in _addr;
};
#endif