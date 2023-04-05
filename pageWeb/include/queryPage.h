#ifndef __QUERYPAGE_H__
#define __QUERYPAGE_H__
#include "pageLib.h"

struct queryResult
{
    string title;
    string link;
    string description;
    string content;
};

class queryPage
{
public:
    queryPage(PageLib &pagelib);
    void wordsSim(string _words, vector<queryResult> &qrs);
    void jiaoji(const vector<string> &, vector<string> &, vector<vector<double>> &);
    void xmlToStr(queryResult &qr, int idx);
private:
    PageLib &_pageLib;
};

#endif


