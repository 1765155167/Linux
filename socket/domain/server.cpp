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
//#define offsetof(type, memer) (int)&((type*)0)->memer
void check(std::string str, int res) 
{
    if(str.compare("unlink"))
    {
        return ;
    }
    if(res < 0)
    {
        perror(str.c_str());
        exit(res);
    }
}

int main()
{
    int clie_fd = 0;
    int serv_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    check("socket", serv_fd);
    struct sockaddr_un serv_addr;
    struct sockaddr_un clie_addr;
    socklen_t serv_len = sizeof(serv_addr);
    socklen_t clie_len = sizeof(clie_addr);
    bzero(&serv_addr, serv_len);    
    bzero(&clie_addr, clie_len);

    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SERV_PATH);
    int len = offsetof(struct sockaddr_un, sun_path) + strlen(serv_addr.sun_path);
    
    unlink(SERV_PATH);
    
    int res = bind(serv_fd, (struct sockaddr*)&serv_addr, len);
    check("bind", res);
    
    res = listen(serv_fd, 128);
    check("listen", res);
    
    while(true)
    {
        clie_len = sizeof(clie_addr);
        clie_fd = accept(serv_fd, (struct sockaddr*)&clie_addr, &clie_len);
        check("accept", clie_fd);
        clie_len -= offsetof(struct sockaddr_un, sun_path);
        clie_addr.sun_path[clie_len] = '\0';
        std::cout << "cient bind " << clie_addr.sun_path << std::endl;
        char buf[1024];
        while(true) {
            len = read(clie_fd, buf, 1024);
            if(len == 0)
            {
                std::cout << "client unlink ..." << std::endl;
                break;
            }
            for(int i = 0; i < len; i++) 
            {
                buf[i] = toupper(buf[i]);
            }
            len = write(clie_fd, buf, len);
            check("write", len);
        }
        close(clie_fd);
    }
    close(serv_fd);
    return 0;
}

