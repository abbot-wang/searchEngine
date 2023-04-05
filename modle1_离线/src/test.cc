#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include "../include/DictProducer.h"
#include "../include/SplitTool.h"

using namespace std;


int main()
{
    SplitTool* tool1 = new CppJieba(); 
    DictProducer dp1("ss",tool1);
    dp1.buildCNDict();

    SplitTool* tool2 = new EnJieba();
    DictProducer dp2("ss",tool2);
    dp2.buildENDict();

    return 0;

}
