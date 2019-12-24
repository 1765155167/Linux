#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "hqf_log.h"
#include "wrap.h"

void waitChild(int no)
{
    if(no == SIGCHLD)
    {
        int status = 0;
        int ret = waitpid(0, &status, WNOHANG);//不堵塞回收所有进程
        if(ret > 0)
        {
            if (WIFEXITED(status) > 0)
            {
                //正常推出
                std::cout << ret << "进程正常推出" << std::endl;
            } else {
                std::cout << ret << "推出信号编号:" << WIFSIGNALED(status) << std::endl;
            }
        }
    }
}

int main()
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = waitChild;
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, NULL);//设置信号捕捉用于回收子进程

    int server_fd = Socket(AF_INET, SOCK_STREAM, 0);
    //设置端口复用
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t server_len = sizeof(server_addr);
    socklen_t client_len = sizeof(client_addr);
    bzero(&server_addr, sizeof(server_addr));
    bzero(&client_addr, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定本地有效的任意IP
    Bind(server_fd, (struct sockaddr*)&server_addr, server_len);

    Listen(server_fd, 128);//设置最大监听数

    while(true) {   
        char IP_BUF[BUFSIZ];
        int client_fd = Accept(server_fd,(struct sockaddr*)&client_addr, &client_len);
        inet_ntop(AF_INET, (void*)&client_addr.sin_addr.s_addr, IP_BUF, client_len);
        std::cout << "client ip:" << IP_BUF << " port:" << ntohs(client_addr.sin_port) << std::endl;
        pid_t pid = fork();
        check("fork", pid);
        if(pid > 0) {
            close(client_fd);
        }else if(pid == 0) {
            close(server_fd);
            while(true) {
                char* buf = new char[BUFSIZ];
                int len = Readline(client_fd, buf);
                if(len == 0)
                {
                    break;
                }
                if((strcmp(buf, "exit\n") == 0) || (strcmp(buf, "exit") == 0))
                {
                    break;
                }
                for(int i = 0; i < len; i++)
                {
                    buf[i] = toupper(buf[i]);
                }
                Write(client_fd, buf, len);
                delete[] buf;
            }
            //close(client_fd);
            exit(-1);
        }
    }
    close(server_fd);
    return 0;
}
