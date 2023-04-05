#include "../include/queryPage.h"
#include "../include/simhash/Simhasher.hpp"
// #include "../include/tinyxml2/tinyxml2.h"
#include <sw/redis++/redis++.h>
#include <regex>
#include <utility>
#include <algorithm>
#include <set>
#include <math.h>
#include <stdlib.h>

using namespace simhash;
using namespace sw::redis;
// using namespace tinyxml2;
using std::pair;
using std::regex;
using std::set;

queryPage::queryPage(PageLib &pagelib)
: _pageLib(pagelib)
{

}
void queryPage::wordsSim(string _words, vector<queryResult> &qrs)
{
    // auto redis1 = Redis("tcp://127.0.0.1:6379/4");
    set<string> dc;

    Simhasher simhasher("../include/simhash/dict/jieba.dict.utf8",
                        "../include/simhash/dict/hmm_model.utf8",
                        "../include/simhash/dict/idf.utf8",
                        "../include/simhash/dict/stop_words.utf8");

    size_t topN = 3;
    vector<pair<string,double>> res;
    simhasher.extract(_words,res,topN);

    vector<string> keys;
    vector<double> keyWeigth;
    for(auto &elem : res)
    {
        keys.push_back(elem.first);
        keyWeigth.push_back(elem.second);
        dc.insert(elem.first);
    }

    vector<string> result;
    vector<vector<double>> weight;
    jiaoji(keys,result,weight);

    vector<pair<string,double>> resPage;
    for(int i = 0; i < weight.size(); ++i)
    {
        double xy = 0, x = 0, y = 0;
        for(int j = 0; j < keyWeigth.size(); ++j)
        {
            xy += weight[i][j] * keyWeigth[j];
            x += weight[i][j] * weight[i][j];
            y += keyWeigth[j] * keyWeigth[j];
        }
        double tmp = xy / (sqrt(x) * sqrt(y));
        resPage.push_back(std::make_pair(result[i],tmp));
    }

    sort(resPage.begin(),resPage.end(),[](pair<string,double> &a,pair<string,double> &b){return a.second > b.second;});
    
    int numtadd = 0;
    for(auto &elem : resPage)
    {
        set<string> dcp = dc;
        // cout << "pageId = " << elem.first << "    weight = " << elem.second << endl;
        queryResult qr;
        xmlToStr(qr,atoi(elem.first.c_str()));

        size_t topN2 = 5;
        vector<pair<string,double>> res2;
        simhasher.extract(qr.content,res2,topN2);
        
        // cout << "1111111111" << endl;
        for(auto &resElem : res2)
        {
            dcp.insert(resElem.first);
        }
        // cout << "2222222222" << endl;
        for(auto &setElem : dcp)
        {
            qr.description += setElem;
            qr.description += "  ";
        }
        qrs.push_back(qr);
        ++numtadd;
        if(numtadd > 10)
        {
            break;
        }
    }
}

void queryPage::jiaoji(const vector<string> &keys, vector<string> &result, vector<vector<double>> &weight)
{
    auto redis = Redis("tcp://127.0.0.1:6379/5");
    redis.zinterstore("jiaoji",keys.begin(),keys.end());
    redis.zrange("jiaoji",0,-1,back_inserter(result));
    for(auto &elem : result)
    {
        vector<double> wei;
        for(auto &key : keys)
        {
            auto num = redis.zscore(key,elem);
            wei.push_back(*num);
        }
        weight.push_back(wei);
    }
    vector<string> jiaoji = {"jiaoji"};
    redis.del(jiaoji.begin(),jiaoji.end());
}

void queryPage::xmlToStr(queryResult &qr, int idx)
{
    // auto st = _pageLib.getFiles();
    // string s = (*(_pageLib.getFiles()))[idx-1].first;
    auto redis1 = Redis("tcp://127.0.0.1:6379/4");
    auto s1 = redis1.get(std::to_string(idx));
    string s = *s1;

    regex regex1("(<url>)(.*)(</url>)");
    std::smatch result;
    std::regex_search(s, result, regex1);
    qr.link = result[2];

    regex regex2("(<title>)(.*)(</title>)");
    std::smatch result2;
    std::regex_search(s, result2, regex2);
    qr.title = result2[2];

    regex regex3("(<content>)(.*)(</content>)");
    std::smatch result3;
    std::regex_search(s, result3, regex3);
    qr.content = result3[2];
}


