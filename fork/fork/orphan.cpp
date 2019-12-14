#include <iostream>
#include <unistd.h>
#include "check.h"

int main()
{
    pid_t pid = fork();
    check("fork", pid);
    if(pid == 0)
    {
        while(1)
        {
            std::cout << "I'am Child Process." << std::endl;
            std::cout << "ID:" << getpid() << " Father ID:" << getppid() << std::endl;
            sleep(1);
        }
    }else 
    {

        std::cout << "I'am Father Process." << std::endl;
        std::cout << "ID:" << getpid() << " Father ID:" << getppid() << std::endl;
    }
    return 0;
}

