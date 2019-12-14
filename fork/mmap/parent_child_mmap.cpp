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
    const char* filePath = "./mmap";
    int fd = open(filePath, O_RDWR | O_CREAT | O_TRUNC, 0644);
    check("open", fd);
    check("truncate", truncate(filePath, 4096));
    check("unlink", unlink(filePath));
    int len = lseek(fd, SEEK_END, 0);//获取文件大小

    //创建内存映射区
    void* ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    char* pt = (char*)ptr;
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }

    pid_t pid = fork();
    check("fork", pid);
    if(pid == 0)
    {
        std::cout << "ptr:" <<  pt << std::endl;
    } else {
        strcpy(pt, "你是我儿子吗?");
        wait(NULL);
    }

    //释放映射区
    int ret = munmap(ptr, len);
    check("munmap", ret);
    close(fd);
    return 0;
}
