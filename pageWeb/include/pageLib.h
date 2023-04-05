#ifndef __PAGELIB_H__
#define __PAGELIB_H__

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <memory>

using std::vector;
using std::map;
using std::pair;
using std::string;
using std::shared_ptr;

class PageLib
{
public:
    PageLib(string);
    void scanXml();
    void create();
    void store();
    shared_ptr<vector<pair<string,int>>> &getFiles();
private:
    string _pwd;
    vector<string> _dirScanner;
    shared_ptr<vector<pair<string,int>>> _files;
    map<int,pair<int,int>> _officeLib;
};

#endif

