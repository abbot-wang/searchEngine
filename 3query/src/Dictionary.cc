#include "../include/Dictionary.h"

Dictionary *Dictionary::_pInstance = nullptr;

Dictionary::Dictionary() 
{
    
}


void Dictionary::init(const string &dictpath)
{
    cout<<">initial dict begin"<<endl;
    MyRedis redis;
    _redis = redis;
    _redis.connect("127.0.0.1", 6379);
    auto fileList = getFileList(dictpath);
    string word;
    string freq;
    _redis.select("0");
    for (auto file : fileList)
    {
        ifstream ifs(file);
        if (!ifs.good())
        {
            cout << ">>open file error ifs.open!" << endl;
            return;
        }
        string line;
        while (getline(ifs, line))
        {
            istringstream is(line);
            while(is)
            {
                is>>word;
                is>>freq;
            }
            _dict.push_back(make_pair(word,freq));
            //_redis.Set(word, freq);
        }
    }

    getIndexTable("/home/pz/C++/搜索引擎项目/在线部分/searchEngine_unjuanable/3query/index/");
    cout<<">initial dict down"<<endl;
}

void Dictionary::getIndexTable(const string &indexpath)
{
    vector<string> fileList = getFileList(indexpath);
    string word;
    string index;
    _redis.select("1");
    for (auto file : fileList)
    {
        ifstream ifs(file);
        if (!ifs.good())
        {
            cout << ">>open file error ifs.open!" << endl;
            return;
        }
        string line;
        while (getline(ifs, line))
        {
            istringstream iss(line);
            if (iss)
            {
                iss >> word;
                while (iss)
                {
                    iss >> index;
                    _redis.sadd(word.c_str(), index.c_str());
                }
            }
        }
    }
    cout<<">load dict down"<<endl;
    cout<<">load indexTable down"<<endl;
}

map<string,string> Dictionary::doQuery(const string &key)
{
    map<string,string> result;
    vector<string> wordlist;
    
    int wordleft=0;
    int idx=0;
    if (is_chinese(key)) //中文
    {
        if(key.length() > 3) //大于一个字情况
        {
            while (wordleft < key.size())
            {
                int len = 3;
                wordlist.push_back(key.substr(wordleft, len));
                if (wordleft == 0) //第一次不做交集运算 将所有索引压入结果集
                {
                    // auto first = _redis.smembers(wordlist[0]);

                    // for (auto it = first.begin(); it != first.end(); ++it)
                    // {
                    //     result.push_back(make_pair(_dict[atoi((*it).c_str()) - 1].first, _dict[atoi((*it).c_str()) - 1].second)); //将结果压入结果集
                    // }
                    wordleft += len;
                    continue;
                }
                else //求交集
                {
                    set<string> inter = _redis.sinter(wordlist[idx], wordlist[idx+1]);
                    for (auto it = inter.begin(); it != inter.end(); ++it)
                    {
                        result.insert(make_pair(_dict[atoi((*it).c_str()) - 1].first, _dict[atoi((*it).c_str()) - 1].second)); //将结果压入结果集
                    }
                    idx++;
                }
                wordleft += len;
            }
        }
        else //单字情况
        {
            vector<string> inter = _redis.smembers(key);
            for (auto it = inter.begin(); it != inter.end(); ++it)
            {
                result.insert(make_pair(_dict[atoi((*it).c_str()) - 1].first, _dict[atoi((*it).c_str()) - 1].second));
            }
        }
    }
    else
    {
        if(key.length() > 1) //大于一个字情况
        {
            while (wordleft < key.size())
            {
                int len = 1;
                wordlist.push_back(key.substr(wordleft, len));
                if (wordleft == 0) //第一次不做交集运算 将所有索引压入结果集
                {
                    wordleft += len;
                    continue;
                }
                else //求交集
                {
                    set<string> inter = _redis.sinter(wordlist[idx], wordlist[idx+1]);
                    for (auto it = inter.begin(); it != inter.end(); ++it)
                    {
                        result.insert(make_pair(_dict[atoi((*it).c_str()) - 1].first, _dict[atoi((*it).c_str()) - 1].second)); //将结果压入结果集
                    }
                    idx++;
                }
                wordleft += len;
            }
        }
        else //单字情况
        {
            vector<string> inter = _redis.smembers(key);
            for (auto it = inter.begin(); it != inter.end(); ++it)
            {
                result.insert(make_pair(_dict[atoi((*it).c_str()) - 1].first, _dict[atoi((*it).c_str()) - 1].second));
            }
        }
    }
    return result;
}

// void Dictionary::queryIndex(const string &key)
// {
// }



vector<string> Dictionary::getFileList(const string &dictpath)
{
    DIR *dir = opendir(dictpath.c_str());
    struct dirent *direntp;
    vector<string> fileList;
    while ((direntp = readdir(dir)) != NULL)
    {
        if (((string)direntp->d_name) != "." && ((string)direntp->d_name) != "..")
            fileList.push_back(dictpath.c_str() + (string)direntp->d_name);
    }

    for (auto &it : fileList)
    {
        cout << it << endl;
    }
    return fileList;
}

bool Dictionary::is_chinese(const std::string &str)
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