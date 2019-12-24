#include "hqf_log.h"

void check(std::string str, int ret) 
{
    if(ret == -1)
    {
        perror(str.c_str());
        exit(-1);
    }
}

void check(std::string str, void* ret, void* errnum)
{
    if(ret == errnum)
    {
        perror(str.c_str());
        exit(-1);
    }
}
