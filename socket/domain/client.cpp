#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/un.h>
#include <string.h>
#include <netinet/ip.h> /* superset of previous */
#define SERV_PATH "serv.socket"
#define CLIE_PATH "clie.socket"
//#define offsetof(type, memer) (int)&((type*)0)->memer
void check(std::string str, int res) 
{
    if(res < 0)
    {
        perror(str.c_str());
        exit(res);
    }
}

int main()
{
    int clie_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    check("socket", clie_fd);
    struct sockaddr_un serv_addr;
    struct sockaddr_un clie_addr;
    socklen_t serv_len = sizeof(serv_addr);
    socklen_t clie_len = sizeof(clie_addr);
    bzero(&serv_addr, serv_len);    
    bzero(&clie_addr, clie_len);
    
    clie_addr.sun_family = AF_UNIX;
    strcpy(clie_addr.sun_path, CLIE_PATH);
    int len = offsetof(struct sockaddr_un, sun_path) + strlen(clie_addr.sun_path);
    unlink(CLIE_PATH);
    int res = bind(clie_fd, (struct sockaddr*)&clie_addr, len);
    check("bind", res);
    
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SERV_PATH);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(serv_addr.sun_path);
    connect(clie_fd, (struct sockaddr*)&serv_addr, len);
    char buf[1024];
    while(true)
    {
        std::cin >> buf;
        write(clie_fd, buf, strlen(buf));
        read(clie_fd, buf, sizeof(buf));
        std::cout  << buf << std::endl;
    }
    close(clie_fd);
    return 0;
}

