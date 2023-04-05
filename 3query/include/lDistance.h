#ifndef __LDISTANCE_H__
#define __LDISTANCE_H__

#include <string>
#include <iostream>

using namespace std;

class LDistance
{
public:
    LDistance();
    int editDistance(const string& lhs,const string& rhs);
private:
    size_t nBytesCode(const char ch);
    size_t length(const string& str);
    int triple_min(const int& a,const int& b,const int& c);

};



#endif 
