#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>

int main()
{
    int fd = 0;
    int ret = 0;
    fd = open("./README.md", O_WRONLY);
    if(fd == -1)
    {
        perror("open");
        exit(-1);
    }
    std::string str1 = "你好请看文件最后~有惊喜^~^!\n";
    ret = write(fd, str1.c_str(), str1.length());
    if(ret == -1)
    {
        perror("write one");
        exit(-1);
    }

    int cmd = fcntl(fd, F_GETFL, 0);
    std::cout << "cmd:" << cmd << std::endl;

    cmd |= O_APPEND;
    ret = fcntl(fd, F_SETFL, cmd);
    if(ret == -1)
    {
        perror("fcntl");
        exit(-1);
    }

    std::string str2 = "你好我是你爸爸!\n";
    ret = write(fd, str2.c_str(), str2.length());
    if(ret == -1)
    {
        perror("write");
        exit(-1);
    }
    close(fd);
    return 0;
}

