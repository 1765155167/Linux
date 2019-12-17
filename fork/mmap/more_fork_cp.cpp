#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "hqf_log.h"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "please input fileone filetwo" << std::endl;
        exit(-1);
    }
    int fdr = open(argv[1], O_RDWR);
    check("open", fdr);
    struct stat sbuf;
    fstat(fdr, &sbuf);
    int len = sbuf.st_size;
    std::cout << "file length :" << len << std::endl;
    int fdw = open(argv[2], O_RDWR | O_CREAT, 0644);
    check("open", fdw);
    ftruncate(fdw, len);//设置文件大小
    //创建内存地址映射
    void* rptr = mmap(NULL, len, PROT_READ, MAP_SHARED, fdr, 0);
    char* rbuf = (char*)rptr;
    if(rptr == MAP_FAILED)
    {
        std::cout << "mmap err" << std::endl;
        exit(1);
    }
    void* wptr = (void*)mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fdw, 0);
    char* wbuf = (char*)wptr;
    if(wptr == MAP_FAILED)
    {
        std::cout << "mmap err" << std::endl;
        exit(1);
    }

    strncpy(rbuf, wbuf, len);
    
    munmap(rbuf, len);
    munmap(wbuf, len);
    close(fdr);
    close(fdw);
    return 0;
}

