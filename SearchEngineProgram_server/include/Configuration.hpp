#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <iostream>
#include <map>
#include <string>

using namespace std;

class Configuration
{
public:
    map<string,string>& getConfigMap()
    {
        return this->_configs;
    }
    static Configuration* getInstance()
    {
        static Configuration *temp;
        return temp;
    }
private:
    Configuration(){}
    ~Configuration(){}
    Configuration(const Configuration&) = delete;
    Configuration &operator=(const Configuration&) = delete;
    string _configFilePath;
    map<string,string> _configs;
};

#endif
