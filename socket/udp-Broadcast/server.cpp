#include <iostream>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

void check(std::string str, int res)
{
    if(res == -1) {
        perror(str.c_str());
        exit(res);
    }
}

int main()
{
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    int n = 220*1024;
    setsockopt(udp_socket, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n));
    int opt = 1;
    setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    check("socket", udp_socket);
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    struct sockaddr_in serv_addr;
    struct sockaddr_in clie_addr;
    socklen_t serv_len = sizeof(serv_addr);
    socklen_t clie_len = sizeof(clie_addr);
    bzero(&serv_addr, serv_len);
    bzero(&clie_addr, clie_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8000);
    inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr.s_addr);
    int res = bind(udp_socket, (struct sockaddr*)&serv_addr, serv_len);
    check("bind", res);
    
    clie_addr.sin_family = AF_INET;
    clie_addr.sin_port = htons(9000);
    inet_pton(AF_INET, "192.168.18.255", &clie_addr.sin_addr.s_addr);
    while(true) {
        static int i = 0;
        sprintf(buf, "hello i'am hqf ip:%d", i++);
        int len = sendto(udp_socket, buf, sizeof(buf), 0, (struct sockaddr*)&clie_addr, clie_len);
        check("sendto", len);
        sleep(2);
    }
    return 0;
}

