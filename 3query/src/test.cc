#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include "../include/Dictionary.h"
#include "../include/KeyRecommander.h"

using namespace std;


int main()
{
    string queryWord;
    KeyRecommander rec("init");
    while(1)
    {
        cout<<"请输入查询词:"<<endl;
        cin>>queryWord;
        cout<<"推荐词"<<"\t\t"<<"频率"<<"\t\t"<<"编辑距离"<<endl;
        rec.doQuery(queryWord);
        cout<<endl;
    }
    


    return 0;
}
