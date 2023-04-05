#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__

#include "CandidateResult.h"
#include "Dictionary.h"
#include "lDistance.h"
#include <queue>
#include <memory>
#include <vector>
//#include "../../../78-6-21/network/TcpConnection.h"

//using TcpConnectionPtr = shared_ptr<TcpConnection>;
using namespace std;

class cmp
{
public:
	bool operator()(CandidateResult &lhs, CandidateResult &rhs)
	{
		if(lhs._dist>rhs._dist)return true;//最小编辑距离越小，优先级越高
		else if(lhs._dist<rhs._dist)return false;
		else return lhs._freq<rhs._freq;//最小编辑距离相同， 词频越高，优先级越高
	}
};

class KeyRecommander
{
public:
    void doQuery(const string &queryword);
    KeyRecommander(string word/*,TcpConnectionPtr conn*/);
    void onConnection();
    void onMessag();
    void onClost();
private:
    string _queryWord;
    //TcpConnectionPtr _conn;
    priority_queue<CandidateResult,vector<CandidateResult>,cmp> _priQueue;
    Dictionary *_dic;
    int distance(const string& lhs, const string& rhs);
};

#endif 
