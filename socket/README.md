# Socket 套接字
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
4. close();//发送结束指令

