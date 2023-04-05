#include <fstream>
#include <codecvt>
#include<regex>
#include "../include/DictProducer.h"

vector<pair<string,int>> _dict;//词典 不放在实现文件

DictProducer::DictProducer(string s,SplitTool* tool)
    :_cutTool(tool)//接收基类指针，实际基类在文件判断时判断指向哪个派生类指
    ,_searchPlace(s)
    ,_isInEN(false)
    ,_idx(1)
{
}

string censorStrWithOutSymbol(const string &source)
{
    string sourceWithOutSymbol;
    // regex reg("[\u4e00-\u9fa5]+");

    // auto word_begin=std::sregex_iterator(source.begin(), source.end(), reg);//正则匹配中文
	// auto word_end=std::sregex_iterator();

    // for(auto it=word_begin;it!=word_end;++it)
    // {
    //     sourceWithOutSymbol+=(*it).str();
    // }

    int i = 0;
    while (source[i] != 0)
    {
        if (source[i] & 0x80 && source[i] & 0x40 && source[i] & 0x20)
        {
            int byteCount = 0;
            if (source[i] & 0x10)
            {
                byteCount = 4;
            }
            else
            {
                byteCount = 3;
            }
            for (int a = 0; a < byteCount; a++)
            {
                sourceWithOutSymbol += source[i];
                i++;
            }
        }
        else if (source[i] & 0x80 && source[i] & 0x40)
        {
            i += 2;
        }
        else
        {
            i += 1;
        }
    }
    auto it = std::remove_if(sourceWithOutSymbol.begin(), sourceWithOutSymbol.end(), ::ispunct);
    sourceWithOutSymbol.erase(it, sourceWithOutSymbol.end());
    return sourceWithOutSymbol;
}


void DictProducer::create()
{
    cout<< ">>in create!" << endl;
    getFileList();
    for(auto file : _filesList)
    {
        ifstream ifs;//追加写但从头开始匹配
        ifs.open(file,ios::in);
        if(!ifs.good())
        {
            cout<<">>open file error ifs.open!"<<endl;
            return;
        }
        ifs.seekg(0,ios::beg);//移到文件开头
        string line;
        while(getline(ifs,line))
        {
            if(_isInEN==false)
            {
                line=censorStrWithOutSymbol(line);
                ifstream CNif("../include/stop_words/stop_words_zh.txt"); //加载停用词
                if (!CNif.good())
                {
                    cout << "文件打开失败！" << endl;
                    system("pause");
                    exit(-1);
                }
                while(!CNif.eof())
                {
                    string CNword;
                    CNif>>CNword;
                    _stopWords.insert(CNword);
                }
            }
            else
            {
                ifstream ENif("../include/stop_words/stop_words_eng.txt"); //加载停用词
                if (!ENif.good())
                {
                    cout << "文件打开失败！" << endl;
                    system("pause");
                    exit(-1);
                }
                while(!ENif.eof())
                {
                    string ENword;
                    ENif>>ENword;
                    _stopWords.insert(ENword);
                }
            }
            vector<string> sLine = _cutTool->cut(line);
            for(auto word = sLine.begin(); word != sLine.end(); word++)
            {
                if(_stopWords.count((*word)))
                {
                    continue;
                }
                _idx=1;//从1开始寻找对应单词 
                for(vector<pair<string,int>>::iterator it = _dict.begin();;it++)
                {             
                    if(it == _dict.end())
                    {
                        _dict.push_back(make_pair(*word,1));
                        break;
                    }
                    if((*it).first == (*word))
                    {
                        
                        (*it).second++;
                        break;
                    }
                    _idx++;
                }
                if (!_isInEN)
                {
                    int wordleft = 0;
                    while (wordleft < (*word).size())
                    {
                        int len = 0;
                        for (int j = 0; j < 6 && ((*word)[wordleft] & (0x80 >> j)); j++)
                        {
                            len = j + 1;
                        }
                        _index[(*word).substr(wordleft, len)].insert(_idx );
                        wordleft += len;
                    }
                }
                else
                {
                    int wordleft = 0;
                    while (wordleft < (*word).size())
                    {
                        int n = 1;
                        _index[(*word).substr(wordleft, n)].insert(_idx );
                        wordleft += n;
                    }
                }
            }
        }
        ifs.close();
    }
}
//建立中文词典
void DictProducer::buildCNDict(){
    _isInEN = false;
    cout<< ">>in buildCNDict!" << endl;
    _searchPlace = "../conf/yuliao/art/";
    cout << _searchPlace << endl;
    create();
    _dictFileName = "../data/_dict.dat";
    _indexFileName = "../data/_dictIndex.dat";
    CNstore();

}
//建立英文词典
void DictProducer::buildENDict(){
    _isInEN = true;
    cout<< ">>in buildENDict!" << endl;
    _searchPlace = "../conf/yuliao/english/";
    cout << _searchPlace << endl;
    create();
    _dictFileName = "../data/_dict.dat";
    _indexFileName = "../data/_dictIndex.dat";
    ENstore();
}

void DictProducer::createIndex(){
    cout<< ">>in createIndex!" << endl;
    ofstream ofss;
    ofss.open(_indexFileName,ios::app|ios::out); 
    if(!ofss.good())
    {
        cout<<">>open file error indexFileName!"<<endl;
        return;
    }
    for(auto it : _index)
    {

        ofss << it.first << " ";
        for(auto itt : it.second)
        {
            ofss << itt << " ";
        }
        ofss << endl;
    }
    ofss.close();
}

void DictProducer::CNstore(){
    cout<< ">>in store!" << endl;
    ofstream ofs;
    ofs.open(_dictFileName,ios::app|ios::out); 
    if(!ofs.good())
    {
        cout<<">>open file error dictFileName!"<<endl;
        return;
    }
    for(auto it : _dict)
    {
        ofs << it.first << "\t" << it.second <<endl;
    }
    createIndex();
    ofs.close();
}

void DictProducer::ENstore(){
    cout<< ">>in store!" << endl;
    ofstream ofs;
    ofs.open(_dictFileName,ios::app|ios::out); 
    if(!ofs.good())
    {
        cout<<">>open file error dictFileName!"<<endl;
        return;
    }
    for(auto it : _dict)
    {
        if(is_chinese(it.first))
        {
            continue;
        }
        ofs << it.first << "\t" << it.second <<endl;
    }
    createIndex();
    ofs.close();
}

void DictProducer::getFileList(){
    DIR* dir = opendir(_searchPlace.c_str());
    struct dirent* direntp;
    while((direntp = readdir(dir)) != NULL)
    {
        if(((string)direntp->d_name) != "." && ((string)direntp->d_name) != "..")
        _filesList.push_back(_searchPlace + (string)direntp->d_name); 
    }
    
    for(auto &it :_filesList)
    {
        cout<<it<<endl;
    }
}

bool DictProducer::is_chinese(const std::string &str)
{
    for (int i = 0; i < str.length(); i++)
    {
        while (str[i] != 0)
        {
            if (str[i] & 0x80 && str[i] & 0x40 && str[i] & 0x20)
            {
                int byteCount = 0;
                if (str[i] & 0x10)
                {
                    byteCount = 4;
                    return true;
                }
                else
                {
                    byteCount = 3;
                    return true;
                }
            }
            else if (str[i] & 0x80 && str[i] & 0x40)
            {
                i += 2;
                return false;
            }
            else
            {
                i += 1;
                return false;
            }
        }
    }
    return false;
}
