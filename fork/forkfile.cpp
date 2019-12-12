#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstring>

int main()
{
    int fd = open("temp.md", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd == -1)
    {
        perror("open");
        exit(-1);
    }
    if(unlink("temp.md") == -1)
    {
        perror("unlink");
        exit(-1);
    }
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        exit(-1);
    }
    if(pid == 0)
    {
        sleep(1);
        lseek(fd, SEEK_SET, 0);//父子进程共享文件指针
        char *buf = new char[128];
        if(read(fd, buf, 128) == -1)
        {
            perror("read");
            exit(-1);
        }
        std::cout << "read:" << buf << std::endl;
    }else{
        const char* buf = "Hello I'am Hqf";
        if(write(fd, buf, strlen(buf)) == -1)
        {
            perror("write");
            exit(-1);
        }
        wait(NULL);
    }
    close(fd);
    return 0;
}
