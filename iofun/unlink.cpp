#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include "check.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main()
{
    //创建文件
    int fd = open("tempfile", O_CREAT | O_RDWR | O_TRUNC, 0644);
    check("open", fd);
    //删除临时文件
    int ret = unlink("tempfile");
    check("unlink", ret);
    //向文件写信息
    const char* wBuf = "Hello I'am Hqf!";
    ret = write(fd, wBuf, strlen(wBuf));
    check("write", ret);
    //改变文件指针
    ret = lseek(fd, SEEK_SET, 0);
    //读文件
    char* buf = new char[128];
    memset(buf, 0, sizeof(char)*128);
    ret = read(fd, buf, 128);
    check("read", ret);
    std::cout << "read file: " << buf << std::endl;
    delete[] buf;
    close(fd);
    return 0;
}

