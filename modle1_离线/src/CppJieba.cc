#include "../include/CppJieba.h"

const char* const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "../include/cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";

CppJieba::CppJieba()
:_jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
{
}

CppJieba::~CppJieba(){
}
vector<string> CppJieba::cut(string& s){
    _jieba.CutForSearch(s,_words);
    return _words;
}


EnJieba::EnJieba()
{
}
EnJieba::~EnJieba()
{
}
void EnJieba::killSymble(string& _line)
{
    for(unsigned int i = 0;i < _line.size();i++)
    {
        if(_line[i] == '\n' || _line[i] == ' '
           || (_line[i] >= 65 && _line[i] <= 90)
           || (_line[i] >= 97 && _line[i] <= 122))
        {
            if(_line[i] >= 65 && _line[i] <= 90)
            {
               _line[i] += 32; 
            }
            continue;
        }        
        else
        {
            _line[i] = ' ';
        }
    }
}

vector<string> EnJieba::cut(string& _line)
{
    killSymble(_line); 
    _lineWord.clear();
    istringstream iss(_line.c_str());
    string word;
    while(iss >> word)
    {

        _lineWord.push_back(word);
    }
    return _lineWord;
}
