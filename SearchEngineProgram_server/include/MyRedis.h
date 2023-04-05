#ifndef __TESTREDIS_H__
#define __TESTREDIS_H__

#include <string.h>
#include <stdio.h>
#include <hiredis/hiredis.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unistd.h>


using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;

class MyRedis
{
public:
    MyRedis() {}
    ~MyRedis()
    {
        _pConnect = nullptr;
        _pReply = nullptr;
    }
    bool connect(string host, int port)
    {
        _pConnect = redisConnect(host.c_str(), port);
        if (_pConnect != nullptr && _pConnect->err)
        {
            cout << "connect error : " << _pConnect->errstr << endl;
            return false;
        }
        return true;
    }
    string get(string key)
    {
        _pReply = (redisReply *)redisCommand(_pConnect, "GET %s", key.c_str());
        string str = _pReply->str;
        freeReplyObject(_pReply);
        _pReply = nullptr;
        return str;
    }
    void Set(string key, string value)
    {
        _pReply = (redisReply *)redisCommand(_pConnect, "SET %s %s", key.c_str(), value.c_str());
        freeReplyObject(_pReply);
        _pReply = nullptr;
    }
    void sadd(string key, string value)
    {
        _pReply = (redisReply *)redisCommand(_pConnect, "SADD %s %s", key.c_str(), value.c_str());
        freeReplyObject(_pReply);
        _pReply = nullptr;
    }
    vector<string> smembers(string key)
    {
        vector<string> result;
        _pReply = (redisReply *)redisCommand(_pConnect, "SMEMBERS %s", key.c_str());
        for(int i=0;i<_pReply->elements;++i)
        {
            result.push_back(_pReply->element[i]->str);
        }
        freeReplyObject(_pReply);
        _pReply = nullptr;
        return result;
    }
    void select(string num)
    {
        _pReply = (redisReply *)redisCommand(_pConnect, "SELECT %s", num.c_str());
        freeReplyObject(_pReply);
        _pReply = nullptr;
    }

    set<string> sinter(string key1,string key2)
    {
        set<string> result;    
        _pReply = (redisReply *)redisCommand(_pConnect, "SINTER %s %s", key1.c_str(),key2.c_str());
        for(int i=0;i<_pReply->elements;++i)
        {
            result.insert(_pReply->element[i]->str);
        }
        _pReply = nullptr;
        return result;
    } 
private:
    redisContext *_pConnect;
    redisReply *_pReply;
};

#endif