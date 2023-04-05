#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SplitTool
{
public:
    virtual vector<string> cut(string&) = 0;
private:
};

#endif

