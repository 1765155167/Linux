#include <sys/mman.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include "hqf_log.h"

int main()
{
    int fd = open("./README.md", O_RDWR);
    check("open", fd);
    int len = lseek(fd, SEEK_END, 0);//获取文件大小

    //创建内存映射区
    void* ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }
    std::cout << (char*)ptr << std::endl;
    int ret = munmap(ptr, len);
    check("munmap", ret);
    close(fd);
    return 0;
}
