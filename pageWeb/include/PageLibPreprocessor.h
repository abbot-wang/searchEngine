#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include "pageLib.h"
/* #include <vector> */
#include <unordered_map>

using std::vector;
using std::unordered_map;

class PageLibPreprocessor
{
public:
    PageLibPreprocessor(PageLib &pl);
    void cutRedundantPages();
    void buildInvertIndexMap();
private:
    unordered_map<string,vector<pair<int,double>>> InvertIndexTable;
    PageLib &_pl;
};

#endif

