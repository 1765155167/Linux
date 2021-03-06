#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include "hqf_log.h"

int main()
{
    const char* filePath = "./temp";
    int fd = open(filePath, O_RDWR);
    check("open", fd);
    check("truncate", ftruncate(fd, 4096));
    int len = lseek(fd, SEEK_END, 0);//获取文件大小

    //创建内存映射区
    void* ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    char* pt = (char*)ptr + 1024;
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }
    while(1)
    {
        std::cout << "ptr:" <<  pt << std::endl;
        sleep(1);
    }

    //释放映射区
    int ret = munmap(ptr, len);
    check("munmap", ret);
    close(fd);
    return 0;
}
