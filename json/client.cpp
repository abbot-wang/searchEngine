#include <nlohmann/json.hpp> // /usr/local/include/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>


using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

using json = nlohmann::json;
struct Data
{
    int length;
    int id;
    string data;
    /* data */
};


void to_json(json &j, const Data &p)
{
    j = json{{"length", p.length},
            {"id", p.id},
            {"data", p.data}};
}

void from_json(const json &j, Data &p)
{
    j.at("length").get_to(p.length);
    j.at("id").get_to(p.id);
    j.at("data").get_to(p.data);

}


void test()
{
    int netFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(netFd < 0)
    {
        perror("socket");
        return;
    }

    struct sockaddr_in serverAddr;
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t addrLen = sizeof(serverAddr);

    if(::connect(netFd, (struct sockaddr*)&serverAddr, addrLen) < 0)
    {
        perror("connect");
        close(netFd);
        return;
    }
    printf("conn has connected!...\n");

    Data d;
    d.length = 10;
    d.id = 1;
    d.data = "HelloWorld";
    json j;
    //json j = d;
    to_json(j,d);//将数据转换成json对象
    string serialized_string = j.dump();
    cout << serialized_string <<endl;
    while(1)
    {
        //send发送数据，若对端断开，操作系同会发送一个SIGPIPE信号，导致程序退出，
        //该信号的默认处理方式是退出当前进程
        //解决方案，send第四个参数，MSG_NOSIGNAL
        //size_t ret = send(netFd, j.dump().c_str(), j.dump().size(), MSG_NOSIGNAL);//对端断开send直接出错退出了
        size_t ret = send(netFd, j.dump().c_str(), j.dump().size(), MSG_NOSIGNAL);//对端断开send直接出错退出了
        if(-1 == ret)
        {
            perror("send -1");
            exit(-1);
        }
        sleep(3);
        char buffer[1024] = {0};
        ret = recv(netFd, buffer, sizeof(buffer), 0);
        if(0 == ret)
        {
            perror("recv 0");

        }else if(-1 == ret)
        {
            perror("recv -1");
        }
        printf(">> recv msg from server: %s\n", buffer);
    }

}


int main()
{
    test();
}