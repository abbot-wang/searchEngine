#include <nlohmann/json.hpp> // /usr/local/include/

#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

using json = nlohmann::json;



struct Data
{
    int length;
    int id;
    string data;
    /* data */
};


//get_to / parse / dump
void to_json(json &j, const Data &p)
{
    j = json{{"length", p.length},
            {"id", p.id},
            {"data", p.data}};
}

void from_json(const json &j, Data &p)
{
    j.at("length").get_to(p.length);
    j.at("id").get_to(p.id);
    j.at("data").get_to(p.data);
/*
    j["length"].get_to(p.length);
    j["id"].get_to(p.id);
    j["data"].get_to(p.data);
*/
}


void test()
{
    Data d;
    d.length = 10;
    d.id = 1;
    d.data = "HelloWorld";
    json j;
    to_json(j,d);//将数据转换成json对象
    string serialized_string =  j.dump();
    cout << serialized_string << endl;
}

//序列化和反序列化
void test1()
{
    Data d1 = {10,1,"helloworld"};//C++结构体格式数据
    json j1 = d1;//转换成json对象,也可以写一个to_json
    //to_json(j1,d1);
    string serialized_string1 =  j1.dump();//json对象转换成json字符串
    cout << serialized_string1 << endl;

    //如何从字符串转换成json对象,由字符串能直接转为json对象
    //Data d2 = j1;
    //cout << d2.data <<endl;
    json j2 = serialized_string1;
    string serialized_string2 = j2.dump();
    cout << serialized_string2 << endl;


    json j3 = json::parse(serialized_string1);//可以直接打印
    cout << "j3 = " << j3 <<endl;
    Data d2;
    d2 = j3;
    //from_json(j3,d2);
}


void test2()
{
    json j1 = json::parse(R"({"length": 10, "id" : 1, "data" :"HelloWorld"})");
    cout << "j1 = " << j1 << endl;

    json j2 = json::parse(R"({"data":"HelloWorld","id":1,"length":10})");
    cout << "j2 = " << j2 << endl;

    char buffer[1024] = "{\"data\":\"HelloWorld\",\"id\":1,\"length\":10}";
    json j3 = json::parse(buffer);
    cout << "j3 = " << j3 << endl;

}

int main()
{
    //test1();
    test2();
}