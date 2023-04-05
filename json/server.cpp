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
    int listenFd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t addrLen = sizeof(serverAddr);

    int opt = 1;
    int ret = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    ret = setsockopt(listenFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    bind(listenFd, (struct sockaddr*)&serverAddr, addrLen);
    listen(listenFd,128); 

    printf("server is listening...\n");

    int netFd = accept(listenFd,nullptr,nullptr);
    printf(">> Conn has connected, fd: %d\n", netFd);

    //收发json格式的数据
    char buffer[1024] = {0};
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        size_t ret = recv(netFd, buffer, sizeof(buffer), 0);
        cout << "<< recv client: " << buffer << endl;
        if(0 == ret)
        {
            perror("recv 0 ");
            exit(-1);
        }
        else if(-1 == ret)
        {
            perror("recv -1 ");
            exit(-1);
        }

        cout << "recv nums = " << ret <<endl;

        //收到json数据包，将其解析出来
        Data d;
        json j1 = json::parse(buffer);
        //d = j;
        from_json(j1,d);
#if 0
        //按照C++风格 进行回写
        memset(buffer,0,sizeof(buffer));
        sprintf(buffer,"length: %d, id: %d, data: %s",d.length,d.id,d.data.c_str());
        ret = send(netFd, buffer,sizeof(buffer), 0);
        cout << "<< send client: " << buffer << endl;
#endif

#if 1
        //按照json格式 进行回写
        json j2;
        to_json(j2,d);
        ret = send(netFd, j2.dump().c_str(), j2.dump().size(), 0);
#endif   
    }


}



int main()
{
    test();

}