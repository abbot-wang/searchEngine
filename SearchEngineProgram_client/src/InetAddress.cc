#include "../include/InetAddress.h"
#include <string.h>
InetAddress::InetAddress(const std::string &ip, unsigned short port)
{  
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addr.sin_port = htons(port);
}
InetAddress::InetAddress(const sockaddr_in &addr):_addr(addr){}

std::string InetAddress::ip() const
{
    return inet_ntoa(_addr.sin_addr);
}
unsigned short InetAddress::port() const
{
    return ntohs(_addr.sin_port);
}
const sockaddr_in &InetAddress::addr() const
{
    return _addr;
}