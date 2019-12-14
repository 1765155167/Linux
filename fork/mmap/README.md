## 内存间映射区
### mmap 创建内存映射区
```c
#include <sys/mman.h>
void* mmap(
    void* addr,//首地址 为NULL时系统自动分配地址
    size_t length,//大小,不能为0,一般为文件的大小
    int prot, //权限 PROT_EXEC PORT_READ(读，必须设置) PORT_WRITE(写) PORT_NONE
    int flags,//标志位参数 MAP_SHARED(共享的，数据会同步到磁盘) MAP_PRIVATE(私有的)
    int fd, //文件描述符 要映射的文件(open)
    off_t offset,//映射文件偏移量(必须是4k的整数倍),一般为0
    );
```
- 返回值:
- * 成功:映射区的首地址
- * 失败:MAP_FAILED:(void\* -1)
### munmap 释放内存映射区
```c
#include <sys/mman.h>
int munmap(void *addr, size_t length);
```
- 参考mmap.cpp代码
### 注意事项
1. 返回值不要进行++操作应该char\* p = (char\*) ptr;
2. open打开文件权限要大于等于mmap映射权限
3. 权限必须具有PROT_READ
4. 偏移量必须是4096(4k)的整数倍
5. 长度不能为0
### 通过open是使用O_CREAT一个新文件来创建映射区
1. open("file", O_CREAT|O_RDWR);//创建新文件
2. lseek(fd, SEEK_SET, 100);//需要给文件增加容量
3. write(fd, "1", 1);
4. truncate(path, length);//改变文件容量方法
### 创建匿名映射区
- 参考anommap.cpp 
### 无血缘关系进程利用内存映射区通信
1. 假设两个程序　a.cpp b.cpp
2. 不能使用匿名映射区域，只能借助磁盘文件映射通信
# 判断文件是否存在
- access("fifo", F_OK);
