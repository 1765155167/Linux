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
    int opt = 1;
    setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    check("socket", udp_socket);
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    struct sockaddr_in clie_addr;
    socklen_t clie_len = sizeof(clie_addr);
    bzero(&clie_addr, clie_len);
    clie_addr.sin_family = AF_INET;
    clie_addr.sin_port = htons(9000);
    inet_pton(AF_INET, "0.0.0.0", &clie_addr.sin_addr.s_addr);
    bind(udp_socket, (struct sockaddr*)&clie_addr, clie_len);
    while(true) {
        int len = recvfrom(udp_socket, buf, 1024, 0, NULL, NULL);
        check("recvfrom",len);
        std::cout << buf << std::endl;
    }
    return 0;
}

