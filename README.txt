4.0：
1.先将离线模块生成
key->modle1_离线
web->pageWeb的test.cc里解注释

在线模块编译：
服务端：
g++ *.cc tinyxml2.cpp -llog4cpp -lpthread -lhiredis -lredis++  -std=c++14 -o server


客户端：
g++ *.cc -o client