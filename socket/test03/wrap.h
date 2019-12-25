#ifndef _WRAP_H_
#define _WRAP_H_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

int Socket(int domain, int type, int protocol);
int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
int Listen(int s, int backlog);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Readn(int fd, void *buf, size_t count);
ssize_t Readline(int fd, void *buf);
ssize_t Write(int fd, const void *buf, size_t count);
ssize_t Writen(int fd, const void *buf, size_t count);

#endif
