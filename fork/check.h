#pragma once
#include <iostream>
#include <string>
void check(std::string str, int ret)
{
    if(ret == -1)
    {
        perror(str.c_str());
        exit(-1);
    }
}
