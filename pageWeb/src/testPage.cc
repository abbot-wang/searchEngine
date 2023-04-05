#include "../include/pageLib.h"
#include "../include/PageLibPreprocessor.h"
#include "../include/queryPage.h"
#include "../include/queryPageWeb.h"
#include <iostream>
#include <string>
#include <vector>
#include <string.h>

#define PAGEPWD "../conf/page/"

using std::cerr;
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

void test()
{
    querypageWeb qpw(PAGEPWD);
    // qpw.start(); //注释掉将不会生成文件（耗时）
    cout << "网页库建立完毕" << endl;

    
    while(1)
    {
        vector<queryResult> qrs;
        string s;
        cin >> s;

        qpw.query(s,qrs);
        // cout << "777777777" << endl;
        for(auto &elem : qrs)
        {
            cout << "标题：" << elem.title << endl;
            cout << "摘要：" << elem.description << endl;
            cout << "链接：" << elem.link << endl;
            //cout << "正文：" << elem.content << endl;
        }
        cout << "*************" << endl;
    }
    
}

int main()
{
    test();
    return 0;
}

