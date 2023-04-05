#include "../include/queryPageWeb.h"
#include <iostream>

querypageWeb::querypageWeb(string pwd)
: _pl(pwd)
, _plp(_pl)
, _qp(_pl)
{

}

void querypageWeb::start()
{
    //扫描路径搜索xml文件
    _pl.scanXml();

    //建立网页库和网页偏移库
    _pl.create();

    //网页库去重
    _plp.cutRedundantPages();

    //网页库和网页偏移库保存到硬盘
    _pl.store();

    //建立倒排索引库并把倒排索引库保存到硬盘和redis服务器
    _plp.buildInvertIndexMap();
}

void querypageWeb::query(string &s, vector<queryResult> &qr)
{
    _qp.wordsSim(s,qr);
}
