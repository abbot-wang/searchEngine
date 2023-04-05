#include "../include/PageLibPreprocessor.h"
#include "../include/simhash/Simhasher.hpp"
#include <math.h>
#include <fstream>
#include <sw/redis++/redis++.h>

using namespace simhash;
using std::ofstream;
using namespace sw::redis;

PageLibPreprocessor::PageLibPreprocessor(PageLib &pl)
: _pl(pl)
{

}

void PageLibPreprocessor::cutRedundantPages()
{
    Simhasher simhasher("../include/simhash/dict/jieba.dict.utf8",
                        "../include/simhash/dict/hmm_model.utf8",
                        "../include/simhash/dict/idf.utf8",
                        "../include/simhash/dict/stop_words.utf8");

    vector<uint64_t> simhashList;
    vector<pair<string,int>> &files = *(_pl.getFiles());
    for(int i = 0; i < files.size(); ++i)
    {
        size_t topN = 300;
        uint64_t u64 = 0;
        vector<pair<string,double>> res;
        simhasher.extract(files[i].first,res,topN);
        simhasher.make(files[i].first,topN,u64);

        bool isequal = false;
        for(auto it = simhashList.begin(); it != simhashList.end(); ++it)
        {
            if(Simhasher::isEqual(u64,*it))
            {
                isequal = true;
                files[i].second = 1;
                break;
            }
        }
        if(!isequal)
        {
            simhashList.push_back(u64);
            double sql = 0;
            for(auto itres = res.begin(); itres != res.end(); ++itres)
            {
                sql += (itres->second) * (itres->second);
            }
            sql = sqrt(sql);
            for(auto &elem : res)
            {
                InvertIndexTable[elem.first].push_back(std::make_pair(i+1,elem.second/sql));
            }
        }
    }
}

void PageLibPreprocessor::buildInvertIndexMap()
{
    ofstream ofs("../data/invertIndex.dat");
    auto redis = Redis("tcp://127.0.0.1:6379/5");
    redis.flushdb();

    for(auto &elem : InvertIndexTable)
    {
        ofs << elem.first << "  ";
        for(auto it = elem.second.begin(); it != elem.second.end(); ++it)
        {
            ofs << it->first << "  " << it->second << "  ";
            // cout << "11111111" << endl;
            redis.zadd(elem.first,std::to_string(it->first),it->second);
            // cout << "22222222222" << endl;
        }
        ofs << endl;
    }
    // cout << "333333333" << endl;
}
