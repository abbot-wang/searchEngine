#include "../include/KeyRecommander.h"


KeyRecommander::KeyRecommander(string word/*,TcpConnectionPtr conn*/)
:_queryWord(word)
/*,_conn(conn)*/
{
    _dic=Dictionary::getInstance();
    _dic->init("../data/dict/");
}


string KeyRecommander::doQuery(const string &queryword)
{
    CandidateResult candiResult;
    while(!_priQueue.empty())
    {
        _priQueue.pop();
    }
    map<string,string> candidate=_dic->doQuery(queryword);
    int idx=0;
    int cnt=0;
    for(auto it=candidate.begin();it!=candidate.end();++it)
    {
        candiResult._word=(*it).first;
        candiResult._freq=atoi((*it).second.c_str());
        candiResult._dist=distance(queryword,(*it).first);
        //cout<<candiResult._word<<endl<<candiResult._freq<<endl<<candiResult._dist<<endl;
        _priQueue.push(candiResult);
    }
    string kMsg;
    kMsg+="\n关键词推荐\n";
    while(!_priQueue.empty()&&cnt<10)
    {
        //cout<<_priQueue.top()._word<<"\t\t"<<_priQueue.top()._freq<<"\t\t"<<_priQueue.top()._dist<<endl;
        kMsg+=_priQueue.top()._word;
        // kMsg+="\t\t";
        // //kMsg+=_priQueue.top()._freq;
        // string res;
        // stringstream ss;
        // ss << _priQueue.top()._freq;
        // ss >> res; //或者 res = ss.str();
        // kMsg+=res;
        kMsg+="\n";
        _priQueue.pop();
        cnt++;
    }
    return kMsg;
}

void KeyRecommander::onConnection(){
    // cout << _conn->toString() << " has connected!" << endl;
}
void KeyRecommander::onMessag(){
    // string msg = _conn->receive();
    // cout << "recv msg : " << msg << endl;
    
    //major task
    //do task
    /* doQuery(); */

    //get result 
}

void KeyRecommander::onClost(){
    // cout << _conn->toString() << " has closed!" << endl;
}


int KeyRecommander::distance(const string &lhs, const string &rhs)
{
    LDistance _distance;
    return _distance.editDistance(lhs, rhs);
}