#include <iostream>
#include <unistd.h>

int main()
{
    while(1)
    {
        std::cout << "Hello world" << std::endl;
        sleep(1);
    }
    return 0;
}

