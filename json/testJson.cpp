#include <nlohmann/json.hpp> // /usr/local/include/
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

using json = nlohmann::json;

//数据协议
struct Player
{
    string name;
    int credits;
    int ranking;
};

#if 1
//将数据格式转为json
void to_json(json &j, const Player &p)
{
    j = json{{"name", p.name},
             {"credits", p.credits},
             {"ranking", p.ranking}};
}
#endif

#if 1
// json数据格式转换回来
void from_json(const json &j, Player &p)
{
    j.at("name").get_to(p.name);
    j.at("credits").get_to(p.credits);
    j.at("ranking").get_to(p.ranking);
}
#endif



void test()
{    
    //数据格式为json
    //多条数据 R"([{}, {}, {}])"_json   
    // json数组
    auto j = R"([
    {"name": "Judd Trump","credits": 1754500,"ranking": 1},
    {"name": "Neil Robertson","credits": 1040500,"ranking": 2},
    {"name": "Ronnie O'Sullivan","credits": 954500,"ranking": 3}
    ])"_json;

    //将json格式数据转换为内存数据格式，并放入vector
    //vector<Player> player = j.get<vector<Player>>();
    vector<Player> players;
    j.get_to(players);

    cout << "name: " << players[2].name << endl;
    cout << "credits: " << players[2].credits << endl;
    cout << "ranking: " << players[2].ranking << endl;
}

void test1()
{
    //解析json格式数据?
    //单条数据 R"({ })"
    //json对象
    auto j = json::parse( R"({"happy": true, "pi" : 3.141})" );
    cout << "j = " << j << endl;//j = {"happy":true,"pi":3.141}

    json j_string = "this is a string";
    auto cpp_string = j_string.get<string>(); //类似test()中将数据放入vector一样
    cout << "cpp_string = " << cpp_string << endl;
}

//json的字符串和cpp的字符都可以直接输出
void test2()
{
    json j_string = "this is a string";
    
    //get_to 和 get 的区别 
    auto cpp_string1 = j_string.get<string>();
    string cpp_string2;
    j_string.get_to(cpp_string2);

    string serialized_string = j_string.dump();//获取数据 j_string ---> cpp_string

    cout << cpp_string1
        << " == " << cpp_string2
        << " == " <<j_string.get<string>() << '\n';
    cout << j_string 
        << " == " << serialized_string << endl;
}


int main()
{
    //test();
    test1();
    //test2();
}