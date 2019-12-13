#include <iostream>
#include <ctime>
#include "includes.h"

int main()
{
    int data[20] = {0};
    srand(static_cast<int>(time(NULL)));
    for(int i = 0; i < 20; i++)
    {
        data[i] = rand() % 100;
    }
    Print(data, 20);
    mySort(data, 20);
    Print(data, 20);
    return 0;
}

