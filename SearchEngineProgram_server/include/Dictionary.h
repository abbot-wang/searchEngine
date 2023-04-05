#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

//#include "../1modle/DictProducer.h"
#include "lDistance.h"
#include "MyRedis.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <fstream>
#include <dirent.h>
#include <sstream>
#include <unistd.h>


using namespace std;


class Dictionary
{

    
public:
    static Dictionary * getInstance()
    {
        if(nullptr==_pInstance){
            _pInstance=new Dictionary();
        }
        return _pInstance;
    }
    static void destory(){
        if(_pInstance){
            delete _pInstance;
        }
    }
    //DictProducer&&);
    map<string,string> doQuery(const string& key);
    void init (const string &dictpath); //初始化词典
    vector<string> getFileList(const string&);
    void  getIndexTable(const string &indexpath);
    bool is_chinese(const std::string &str);

private:
    Dictionary();
    ~Dictionary()
    {
        _redis.~MyRedis();
    }
private:
    MyRedis _redis;
    vector<pair<string,string>> _dict;
    map<string,set<int>> _indexTable;
    vector<string> _queryResult;
    static Dictionary * _pInstance;
    //void queryIndex(const string& key);
    // vector<pair<string,int>> _dictEN;
    // map<string,set<int>> _indexEN;
    // vector<pair<string,int>> _dictCN;
    // map<string,set<int>> _indexCN;
    /* vector<string> _linesEN; */
    /* vector<string> _linesCN; */
};
#endif 