# Socket 套接字
## TCP协议
### 三次握手
SYN ACK 
### 四次握手
FIN ACK Linux支持半关闭
### MTU 最大传输单元
### mss 最大报文段长度
### 滑动窗口：流量控制
### TCP状态转换
```shell
# 查看TCP状态
netstat -apn | grep 6666
```
1. ESTABLISHED 三次握手建立完成 进入数据传输状态
2. FIN_WAIT2 主动发起关闭请求病收到ack进入半关闭状态
3. TIME_WAIT 等待2MSL(大约1min)超时，目的是不确定对方是都收到我发送的ack
---
## Linux文件类型
1. C 字符设备文件
2. B 快设备文件
3. P 管道（伪文件）（半双工）
4. S 套接字 （伪文件）（全双工）
## 套接字
1. socket一定是成对出现
2. 绑定 IP:端口号
3. 一个文件描述符指向两个缓冲区（从而实现全双工通信）
## 大端小端模式
大端模式：低地址存放高位数据
小端模式：低地址存放低位数据
|------------|-----------|
|interX86平台| 为小端模式|
|网络数据    | 为大端模式|
## 网络字节序转换
```c
#include <arpa/inet.h>
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
//h:host n:net l:long s:short 32:ip地址 16:端口号
//p:IP n:net "192.168.1.24"-->unsigned int--->htonl
```
## IP地址转换函数
```c
#include <arpa/inet.h>
//p:IP n:net "192.168.1.24"-->网络字节序
int inet_pton(
    int af,//AF_INET IPv4 AF_INET6 IPv6
    const char *src,//"192.168.1.0"
    void *dst//得到的网络字节序
);
//网络字节序号---》点分十进制
const char *inet_ntop(
    int af,
    const void *src,//网络字节序
    char *dst, //得到的点分十进制"192.168.1.1"
    socklen_t size//网络字节序号大小
);
```
```C
struct sockaddr_in addr;
addr.sin_family = AF_INET/AF_INET6;
addr.sin_port = htons();
addr.sin_addr.s_addr = htonl();
bind(, (struct sockaddr *)&addr);
```
## socket函数
### socket 创建套接字 
- int socket(int domain, int type, int protocol);
- 返回值失败-1, 成功:套接字描述符
### bind 绑定
### listen 指定监听上限数n（同时允许有n个客户端建立链接）
### accept 接收一个客户端的链接请求
- int accept(int sockfd, struct sockaddr\* addr, socklen_t\* addrlen);
- addr: 传出参数代表客户端的addr
- addrlen：传入addr大小 传出参数
- return:成功返回socket文件描述符号（用于与客户端的通信）
### connect 建立链接
- int connect(int sockfd, const struct sockaddr\* addr, socklen_t addrlen);
- return:成功0 失败-1
## CS模型流程图
### 服务器
1. socket();//创建套接字
2. bind();//服务器一般需要手动绑定IP与端口号，这样客户端好访问
3. listen();//最大同时发起链接的数量
4. accept();//堵塞等待客户端发起链接
5. read();write();//与客户端通信
6. read();读取客户端发送取消链接指令
7. close();//关闭与客户端的链接
### 客户端
1. socket();
2. bind();//系统会自动绑定ip与端口号,可以不绑定
2. connect();//发起链接
3. write(); read();//通信
4. close();//发送结束指令 关闭指向套接字文件的文件描述符。
5. shutdown(fd, SHUT_RD/SHUT_RDWR/SHUT_WR);关闭套接字 影响其他进程
## 错误处理
### read返回值:
1. ret > 0: 实际读到的字节数
2. ret = 0: 数据读完了或者读到文件末尾，管道写端被关闭，socket末尾:对端关闭
3. ret = -1:异常会置errno，
- 1. errno == EINTR 被信号中断应该重新读
- 2. errno == EAGAIN(EWOULDBLOCK) 非堵塞方式读并且没有数据
- 3. 其他值:真正的发生了错误应该perror();exit();
### write返回值
1. ret > 0: 实际写的字节数
2. ret = 0：写了一个空
3. ret = -1:异常并且置errno
- 1. errno == EINTR 被信号打断，应该重新写
- 2. 其他：发生了错误 perror();exit();
## 端口复用
### getsockopt
```c
int getsockopt(int sockfd, int level, int optname,
               void *optval, socklen_t *optlen);
```
### setsockopt
```c
int setsockopt(int sockfd, int level, int optname,
               const void *optval, socklen_t optlen);
//设置端口可复用
int opt = 1;
int setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
```
## IO多路复用SELECT
```c
int select(int nfds, fd_set *readfds, fd_set *writefds,
        fd_set *exceptfds, struct timeval *timeout);
```
**参数**
1. nfds 监听所有文件描述符中最大的文件描述符**+ 1**
2. readfds 所监听的文明描述符**可读**事件
3. writefds 所监听的文明描述符**可写**事件 
4. exceptfds 所监听的文明描述符**异常**事件
5. timeout 等待的时间
**返回值**
1. 成功：所监听的所有的监听集合中满足条件的**总数**
2. 失败：-1
```c
void FD_CLR(int fd, fd_set *set); //将fd从set中清除
int  FD_ISSET(int fd, fd_set *set);//判断fd是否在集合中
void FD_SET(int fd, fd_set *set);//将fd设置到set集合中
void FD_ZERO(fd_set *set); //将set清空成0值
```
-------------------------------------------------------

### 流程
```c
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(fd1, &readfds);
FD_SET(fd2, &readfds);
FD_SET(fd3, &readfds);
int sum = select();
for(int i = 0; i < sum; i++)
{
    if(FD_ISSET(fd1, &readfds) != 0)
    {
        //fd1读事件
    }
}
```
