#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include "check.h"

int main()
{
    const char* filePath = "./lseek.md";
    int fd = open(filePath, O_RDWR | O_CREAT | O_TRUNC, 0644);
    check("open", fd);
    const char* wBuf = "Hello I'am Hqf";
    int ret = write(fd, wBuf, strlen(wBuf));
    check("write", ret);
    std::cout << "写后直接读文件指针在末尾读不出来" << std::endl;
    char* rBuf = new char[128];
    memset(rBuf, 0, sizeof(char)*128);
    ret = read(fd, rBuf, 128);
    check("read", ret);
    std::cout << "rBuf:" << rBuf << std::endl;
    std::cout << "lseek改变文件指针位置" << std::endl;
    ret = lseek(fd, SEEK_SET, 0);
    check("lseek", ret);
    ret = read(fd, rBuf, 128);
    check("read", ret);
    std::cout << "rBuf:" << rBuf << std::endl;
    
    delete[] rBuf;
    close(fd);
    return 0;
}

