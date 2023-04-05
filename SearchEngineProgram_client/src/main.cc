#include "../include/ClientConnect.h"
#include"../include/JsonTrans.h"
#include<iostream>
#include<string.h>

using std::cout;
using std::endl;

int main()
{
    ClientConnect connect("127.0.0.1", 8888);
    system("clear");
    puts("\033[1;35m===============SearchEngine_Unjuanable================ \033[m ");
    cout<<"请输入类型: 1.关键字推荐 2.网页库查询"<<endl;
    while (true)
    {
        
        printf("\033[1;34m[@Searching:] \033[m ");
        //读取输入内容
        string msg;
        getline(std::cin, msg);

        if (strcmp(msg.c_str(), "clear") == 0)
        {
            system("clear");
            continue;
        }
        else if (strcmp(msg.c_str(), "help") == 0)
        {
            puts("._______________________.");
            puts("|\033[1;35m====操作帮助文档集====\033[m |");
            
            puts("|_______________________|");
            continue;
        }
        char c_msg[1024]="";
        strcpy(c_msg,msg.c_str());
        c_msg[strlen(msg.c_str())]='\n';//将传输的数据末尾加上\n，来适应服务端
        string newMsg(c_msg);

        //打包成json
        Data dMsg;
        dMsg.length=newMsg.size();
        dMsg.ss=newMsg;
        jsonTrans jTrans(dMsg);//dMsg->jTrans._data

        
        //发送json
        connect.sendMessage(jTrans);//真正发送的是字符串
        //接收json
        json retjMsg = connect.recvMessage();
        
        //清空json并接收
        bzero(&dMsg,sizeof(dMsg));
        jTrans.from_json(retjMsg);
        jTrans.printData();
        //cout<<retjMsg<<endl;
    }
}