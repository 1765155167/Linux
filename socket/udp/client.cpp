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
    check("socket", udp_socket);
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    struct sockaddr_in serv_addr;
    socklen_t serv_len = sizeof(serv_addr);
    bzero(&serv_addr, serv_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    while(true) {
        std::cin >> buf;
        int len = sendto(udp_socket, buf, 1024, 0, (struct sockaddr*)&serv_addr, serv_len);
        check("sendto", len);
        
        len = recvfrom(udp_socket, buf, len, 0, NULL, NULL);
        check("recvfrom",len);
        std::cout << buf << std::endl;
    }
    return 0;
}

