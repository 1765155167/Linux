#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "hqf_log.h"

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    check("socket", server_fd);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6666);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定本地有效的任意IP
    bzero(&server_addr.sin_zero,8);
    int ret = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    check("bind", ret);

    listen(server_fd, 128);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    while(true) {   
        int client_fd = accept(server_fd,(struct sockaddr*)&client_addr, &client_len);
        check("accpet", client_fd);
        pid_t pid = fork();
        check("fork", pid);
        if(pid > 0) {
            int status = 0;
            int ret = waitpid(-1, &status, WNOHANG);//不堵塞回收所有进程
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
        }else if(pid == 0) {
            char* ipbuf = new char[50];
            //check("inet_ntop", (void*)inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipbuf, sizeof(client_addr)), NULL);
            ipbuf = inet_ntoa(client_addr.sin_addr);
            std::cout << "link " << ipbuf << std::endl;
            while(true) {
                char* buf = new char[1024];
                ret = read(client_fd, buf, sizeof(1024));
                check("read", ret);
                if(strcmp(buf, "exit") == 0)
                {
                    break;
                }
                for(int i = 0; i < ret; i++)
                {
                    buf[i] = toupper(buf[i]);
                }
                ret = write(client_fd, buf, ret);
                check("write", ret);
                delete[] buf;
            }
            delete[] ipbuf;
            close(ret);
            exit(-1);
        }
    }
    return 0;
}
