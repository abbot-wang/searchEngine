#ifndef __QUERYPAGEWEB_H__
#define __QUERYPAGEWEB_H__

#include "pageLib.h"
#include "PageLibPreprocessor.h"
#include "queryPage.h"

class querypageWeb
{
public:
    querypageWeb(string pwd);
    void start();
    void query(string &s, vector<queryResult> &qr);

private:
    PageLib _pl;
    PageLibPreprocessor _plp;
    queryPage _qp;
};

#endif
