#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include "check.h"

class Print{
public:
    void operator()(int val)
    {
        sleep(1);
        std::cout << val << " ";
    }
};

int main()
{
    std::cout << "start into man." << std::endl;
    pid_t id = fork();
    check("fork", id);
    if(id == 0)
    {
        sleep(1);
        std::cout << "child process..." << std::endl;
        std::cout << "child self id:" << getpid() << std::endl;
        std::cout << "child's father id:" << getppid() << std::endl;
    }else
    {
        std::cout << "father process..." << std::endl;
        std::cout << "father self id:" << getpid() << std::endl;
        std::cout << "father's child id:" << id << std::endl;
    }
    std::vector<int> v(10,5);
    for_each(v.begin(), v.end(), Print());
    std::cout << std::endl;
    return 0;
}

