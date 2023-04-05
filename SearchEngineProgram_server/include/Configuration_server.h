#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include<map>
#include<set>
#include<string>

using std::map;
using std::set;
using std::string;

class Configuration
{
public:
    Configuration(const string& filepath);
    map<string,string> &getConfigMap();//获取存放配置文件内容的map
    set<string> getStopWordList();//获取停用词词集
private:
    string _filepath;//配置文件路径
    map<string,string> _configMap;//配置文件内容
    set<string> _stopWordList;//停用词词集
};




#endif