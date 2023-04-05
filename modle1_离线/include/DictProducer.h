#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include <iostream>
#include "SplitTool.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include "CppJieba.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;



class DictProducer
{
    friend string censorStrWithOutSymbol(const string &source);
public:
    DictProducer(string s,SplitTool* tool);//这个s传的应该是文件路径
    void buildENDict();
    void buildCNDict();
    void createIndex();
    void create();
    void CNstore();
    void ENstore();
    void getFileList();
    int CountLines(const string &);
    bool is_chinese(const std::string &str);
private:
    SplitTool* _cutTool;
    string _searchPlace;
    map<string,set<int>> _index;//索引
    
    set<string> _stopWords;
    vector<string> _filesList;//这个保存的应该时读取的当前目录的文件清单
    string _dictFileName;
    string _indexFileName;
    int _idx;
    bool _isInEN;
};

#endif
