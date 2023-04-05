#ifndef __ERRORCHECK_H__
#define __ERRORCHECK_H__

#include <stdio.h>
#include <stdlib.h>
inline void errorCheck(bool error_cond, const char *msg)
{
    if (error_cond)
    {
        perror(msg);
        exit(-1);
    }
}

#endif