#include <iostream>
#include <wait.h>
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
    struct stat sbuf;
    int fdr = open(argv[1], O_RDWR);
    check("open", fdr);
    fstat(fdr, &sbuf);
    int len = sbuf.st_size;

    int fdw = open(argv[2], O_RDWR | O_CREAT, 0644);
    check("open", fdw);
    ftruncate(fdw, len);//设置文件大小
    
    //创建内存地址映射
    void* rptr = mmap(NULL, len, PROT_READ, MAP_SHARED, fdr, 0);
    char* rbuf = (char*)rptr;
    check("mmap", rptr, MAP_FAILED);
    
    void* wptr = (void*)mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fdw, 0);
    char* wbuf = (char*)wptr;
    check("mmap", wptr, MAP_FAILED);
    
    int n = 5;//默认5个进程进行拷贝
    if(len < n)
        n = len;
    int bs = len / n;//每个进程应拷贝的字节数
    int mod = len % bs;//剩下的让最后一个进程拷贝

    //创建进程
    pid_t pid = 0;
    int i = 0;
    for(i = 0; i < n; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            break;
        }
    }
    
    if(i == n) {//父进程
        for(i = 0; i< n; i++)
        {
            wait(NULL);
        }
    } else if(i == (n - 1)) {//最后一个进程
        memcpy(wbuf+i*bs, rbuf+i*bs, bs+mod);
    } else {//其他进程
        memcpy(wbuf+i*bs, rbuf+i*bs, bs);
    }

    munmap(rbuf, len);
    munmap(wbuf, len);
    close(fdr);
    close(fdw);
    return 0;
}
