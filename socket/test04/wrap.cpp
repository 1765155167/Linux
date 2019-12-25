#include <iostream>
#include "wrap.h"
#include "hqf_log.h"

int Socket(int domain, int type, int protocol)
{
    int ret = socket(domain, type, protocol);
    check("socket", ret);
    return ret;
}

int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen)
{
    int ret = bind(sockfd, my_addr, addrlen);
    check("bind", ret);
    return ret;
}

int Listen(int s, int backlog)
{
   int ret =  listen(s, backlog);
   check("listen", ret);
   return ret;
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
AGAIN:
    int ret = accept(s, addr, addrlen);
    if(ret < 0)
    {
        if(errno == EINTR || errno == ECONNABORTED)
        {
            goto AGAIN;
        }else {
            check("accept", ret);
        }
    }
    return ret;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
AGAIN:
    int ret = connect(sockfd, addr, addrlen);
    if(ret < 0)
    {
        if(ret == EINTR)
            goto AGAIN;
        else
            check("connect", ret);
    }
    return ret;
}

ssize_t Read(int fd, void *buf, size_t count)
{
AGAIN:
    int ret = read(fd, buf, count);
    if(ret < 0)
    {
        if(errno == EINTR || errno == EWOULDBLOCK)
        {
            goto AGAIN;
        }
        if(errno == ECONNRESET)
        {
            close(fd);
            std::cout << "client please reset" << std::endl;
        }
    }else if(ret == 0)
    {
        std::cout << "对方断开链接" << std::endl;
    }
    //std::cout << "read size:" << ret << " buf:" << (char*)buf << std::endl;
    return ret;
}

ssize_t Readn(int fd, void *buf, size_t count)
{
    size_t nleft = count;//剩余未读字节数
    size_t nread = 0;//实际读到字节数
    char *ptr = (char*)buf;//读取位置
    
    while(nleft > 0)
    {
        nread = Read(fd, ptr, nleft);
        if(nread == 0)
        {
            break;
        }
        nleft -= nread;//剩余未读字节
        ptr += nread;//移动读取位置
    }
    return count - nleft;
}

#define LINE_MAX BUFSIZ
ssize_t Readline(int fd, void *buf)
{
    char c;
    char* ptr = (char*)buf;
    int nread = 0;//读取长度
    while(nread < LINE_MAX)
    {
        int ret = Read(fd, &c, 1);//读取一个字符
        if(ret == 0)
        {
            break;
        }
        *ptr++ = c;
        nread++;
        if(c == '\n')
        {
            break;
        }else if(c == '\0')
        {
            return nread - 1;
        }
    }
    *ptr = '\0';
    return nread;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
AGAIN:
    int ret = write(fd, buf, count);
    if(ret < 0)
    {
        if(errno == EINTR)
        {
            goto AGAIN;
        }
        else {
            check("write", ret);
        }
    }
    //std::cout << "write size:" << ret << " buf:" << (char*)buf << std::endl;
    return ret;
}

ssize_t Writen(int fd, const void *buf, size_t count)
{
    size_t nleft = count;//剩余未读字节数
    size_t nwrite = 0;//实际读到字节数
    char *ptr = (char*)buf;//读取位置

    while(nleft > 0)
    {
        nwrite = Write(fd, ptr, nleft);
        nleft -= nwrite;
        ptr += nwrite;
    }
    return count;
}
