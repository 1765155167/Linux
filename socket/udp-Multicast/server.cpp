#include <iostream>
#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    int serv_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serv_addr;
    struct sockaddr_in clie_addr;
    socklen_t serv_len = sizeof(serv_addr);
    socklen_t clie_len = sizeof(clie_addr);
    bzero(&serv_addr, serv_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6666);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(serv_fd, (struct sockaddr*)&serv_addr, serv_len);
    /*设置组地址*/
    struct ip_mreqn group;
    inet_pton(AF_INET, "239.0.0.2", &group.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);
    group.imr_ifindex = if_nametoindex("enos22");
    setsockopt(serv_fd, IPPROTO_IP, IP_MULTICAST_IF, &group, sizeof(group));
    
    /*构造client地址ip+端口*/
    bzero(&clie_addr, clie_len);
    clie_addr.sin_family = AF_INET;
    clie_addr.sin_port = htons(9000);
    inet_pton(AF_INET, "239.0.0.2", &clie_addr.sin_addr.s_addr);
    
    char buf[1024];
    while(true)
    {
        static int i = 0;
        sprintf(buf, "hello i'am huqf %d\n", i++);
        sendto(serv_fd, buf, strlen(buf), 0, (struct sockaddr*)&clie_addr, clie_len);
        sleep(1);
    }

    return 0;
}

