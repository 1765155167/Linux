#include <iostream>
#include <cmath>
#include <algorithm>
#include "includes.h"

void mySort(int data[], int len)
{
    int flag = false;
    for(int i = 1; i < len; i++)
    {
        flag = false;
        for(int j = 0; j < len - i; j++)
        {
            if(data[j] < data[j + 1])
            {
                flag = true;
                std::swap(data[j], data[j + 1]);
            }
        }
        if(!flag)
        {
            break;
        }
    }
}

