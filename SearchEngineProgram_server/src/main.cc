#include "../include/SearchEngineServer.h"

int main()
{
    SearchEngineServer server(5,10,"127.0.0.1",8888);//为测试并发，线程数1，任务队列大小2
    server.start();
    return 0;
}