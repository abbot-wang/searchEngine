#ifndef __JSONTRANS_H__
#define __JSONTRANS_H__


#include"../include/nlohmann/json.hpp"
#include<string>
#include<iostream>

using std::cout;
using std::endl;
using std::string;

using json = nlohmann::json;

//传输火车
struct Data
{
    int length;
    //int id;
    string ss;
    /* data */
};

class jsonTrans
{
public:
    jsonTrans(Data &data)
    {
        _data=data;
    }
    void to_json(json &j)//变成json包
    {
        j = json{{"length", _data.length},
                 //{"id", p.id},
                 {"data", _data.ss}};
    }

    void from_json(const json &j)//解json包
    {
        j.at("length").get_to(_data.length);
        //j.at("id").get_to(p.id);
        j.at("data").get_to(_data.ss);
    }

    void printData()
    {
        cout<<"length="<<_data.length<<endl;
        //cout<<"id="<<_data.id<<endl;
        cout<<"ss="<<_data.ss<<endl;
    }

    int getDataLength()
    {
        return _data.length;
    }

    string getDatass()
    {
        return _data.ss;
    }
private:
    Data _data;    
};



#endif