#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "check.h"

int main()
{
    int fd = 0;
    int ret = 0;
    fd = open("dup.md", O_WRONLY | O_CREAT);
    check("open", fd);
    
    std::string str1 = "你好请看文件最后~有惊喜^~^!\n";
    ret = write(fd, str1.c_str(), str1.length());
    check("write", ret);

    std::cout << "标准输出重定向前" << std::endl;
    ret = dup2(fd, 1);
    check("dup2", ret);
    std::cout << "标准输出重定向后" << std::endl;
    
    close(fd);
    return 0;
}

