#include <iostream>
#include "includes.h"

void Print(int data[], int len)
{
    for(int i = 0; i < len; i++)
    {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

