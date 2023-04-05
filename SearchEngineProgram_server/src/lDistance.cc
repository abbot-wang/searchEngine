#include "../include/lDistance.h"


LDistance::LDistance(){}

int LDistance::editDistance(const string& lhs,const string& rhs){
    size_t lhsLen = length(lhs);
    size_t rhsLen = length(rhs);
    int editDist[lhsLen + 1][rhsLen + 1];
    for(size_t idx = 0;idx <= lhsLen; idx++)
    {
        editDist[idx][0] = idx;
    }
    for(size_t idx = 0;idx <= rhsLen; idx++)
    {
        editDist[0][idx] = idx;
    }

    string sublhs,subrhs;

    for(size_t distI = 1,lhsIdx = 0;distI <= lhsLen; distI++ , lhsIdx++)
    {
        size_t nBytes = nBytesCode(lhs[lhsIdx]);
        sublhs = lhs.substr(lhsIdx,nBytes);
        lhsIdx += (nBytes - 1);

        for(size_t distJ = 1,rhsIdx = 0; distJ <= rhsLen; distJ++ , rhsIdx++)
        {
            nBytes = nBytesCode(rhs[rhsIdx]);
            subrhs = rhs.substr(rhsIdx,nBytes);
            rhsIdx += (nBytes - 1);
            if(sublhs == subrhs)
            {
                editDist[distI][distJ] = editDist[distI - 1][distJ - 1];
            }
            else
            {
                editDist[distI][distJ] = triple_min(editDist[distI][distJ - 1] + 1 ,
                                                    editDist[distI - 1][distJ] + 1 ,
                                                    editDist[distI - 1][distJ - 1] + 1);
            }
        }
    }
    return editDist[lhsLen][rhsLen];
}

size_t LDistance::nBytesCode(const char ch){
    if(ch&(1<<7))
    {
        int nBytes = 1;
        for(int idx = 0; idx != 6 ; idx++)
        {
            if(ch & (1<<(6-idx)))
            {
                ++nBytes;
            }
            else
            {
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

size_t LDistance::length(const string& str){
    size_t ilen = 0;
    for(size_t idx = 0; idx != str.size();idx++)
    {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ilen++;
    }
    return ilen;
}

int LDistance::triple_min(const int& a,const int& b,const int& c)
{
    return a<b?(a<c?a:c):(b<c?b:c);
}
