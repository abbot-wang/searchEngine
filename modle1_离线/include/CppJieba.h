#ifndef __CPPJIEBA_H__
#define __CPPJIEBA_H__

#include <ctime>
#include <fstream>
#include "cppjieba/Jieba.hpp"
#include "SplitTool.h"

using namespace cppjieba;


class CppJieba
:public SplitTool
{
public:
    CppJieba();
    ~CppJieba();
    virtual vector<string> cut(string&);
private:
    vector<string> _words;
    vector<cppjieba::Word> _jiebaWords;
    string _result;
    cppjieba::Jieba _jieba;
    vector<string> _lineWord;
};

class EnJieba
:public SplitTool
{
public:
    EnJieba();
    ~EnJieba();
    void killSymble(string&);
    virtual vector<string> cut(string&);
private:
    vector<string> _lineWord;
};


#endif

