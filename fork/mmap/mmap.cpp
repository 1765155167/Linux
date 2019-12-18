#include <sys/mman.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "hqf_log.h"

int main()
{
    struct stat bufstat;
    int fd = open("./README.md", O_RDWR);
    check("open", fd);
    fstat(fd, &bufstat);
    int len = bufstat.st_size;
    std::cout << "length:" << len << std::endl;

    //创建内存映射区
    void* ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    char* pt = (char*)ptr;
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }
    std::cout << pt << std::endl;

    //释放映射区
    int ret = munmap(ptr, len);
    check("munmap", ret);
    close(fd);
    return 0;
}