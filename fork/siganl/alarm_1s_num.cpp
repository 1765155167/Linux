#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <signal.h>

bool flags = true;

void myfun(int no)
{
    flags = false;
    std::cout << "捕捉到信号:" << no << std::endl;    
}

int main()
{
    signal(SIGALRM, myfun);
    alarm(1);
    int i = 0;
    while(flags)
    {
        i++;
    }
    std::cout << "1s计算次数:" << i << std::endl;
    return 0;
}

