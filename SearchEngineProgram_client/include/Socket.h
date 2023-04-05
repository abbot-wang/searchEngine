#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "NonCopyable.h"

class Socket
: NonCopyable
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int fd() const;
    void shutDownWrite();

private:
    int _fd;
};

#endif
