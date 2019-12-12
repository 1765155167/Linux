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
    pid_t pid = 0;
    int i = 0;
    for(; i < 3; i++) {
        pid = fork();
        if(pid == -1) {
            perror("fork");
            exit(-1);
        }
        if(pid == 0) {
            break;
        }
    }
    switch(i)
    {
    case 0:
        execlp("ps", "ps", "-a", NULL);
        perror("execlp");
        exit(-1);
        break;
    case 1:
        execl("/home/hqf/Linux/fork/file", "file", NULL);
        perror("execl");
        exit(-1);
        break;
    case 2:
        {
            int *data = (int *)0x12340000;
            data[50] = 10;
            for(int i = 0; i < 5; i++)
            {
                std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        }
        break;
    case 3:
        while(true)
        {
            int status = 0;
            //int ret = wait(&status);
            int ret = waitpid(-1, &status, WNOHANG);
            if(ret == -1) {
                break;
            }
            if(ret == 0) {
                continue;
            }
            std::cout << "child pid:" << ret << std::endl;
            if(WIFEXITED(status))
            {
                std::cout << "正常退出" << WIFEXITED(status) << std::endl;
            }else if(WIFSIGNALED(status)){
                std::cout << "信号终止" << WTERMSIG(status) << std::endl;
            }
        }
        break;
    default:
        break;
    }

    return 0;
}
