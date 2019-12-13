#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "check.h"

/*
 * stat 具有穿透能力查看软连接文件时查看的时软连接指向的文件书属性
 * lstat 不具有穿透能力 查看的事软连接文件本身属相信息
 * */

//打印文件属性
void PrintAuthority(int mode)
{
    char perms[9];
    perms[0] = (mode & S_IRUSR) ? 'r':'-';
    perms[1] = (mode & S_IWUSR) ? 'w':'-';
    perms[2] = (mode & S_IXUSR) ? 'x':'-';

    perms[3] = (mode & S_IRGRP) ? 'r':'-';
    perms[4] = (mode & S_IWGRP) ? 'w':'-';
    perms[5] = (mode & S_IXGRP) ? 'x':'-';

    perms[6] = (mode & S_IROTH) ? 'r':'-';
    perms[7] = (mode & S_IWOTH) ? 'w' :'-';
    perms[8] = (mode & S_IXOTH) ? 'x':'-';

    for(int i = 0; i < 9; i++)
    {
        std::cout << perms[i];
    }
    std::cout << std::endl;
}

void PrintStat(struct stat* buf)
{
    std::cout << "/* ID of device containing file */" << buf->st_dev << std::endl
        << "/* inode number */ " << buf->st_ino << std::endl
        << "/* file type and mode */ " << setiosflags(std::ios::hex) << buf->st_mode << std::endl
        << "/* number of hard links */" << buf->st_nlink << std::endl
        << "/* user ID of owner */" << buf->st_uid << std::endl
        << "/* group ID of owner */" << buf->st_gid << std::endl
        << "/* device ID (if special file) */" << buf->st_rdev << std::endl
        << "/* total size, in bytes */ " << buf->st_size << std::endl
        << "/* blocksize for filesystem I/O */" << buf->st_blksize << std::endl     /* blocksize for filesystem I/O */
        << "/* number of 512B blocks allocated */" << buf->st_blocks << std::endl;       /* number of 512B blocks allocated */
    PrintAuthority(buf->st_mode);
}


int main(int argc, char* argv[])
{
    const char* filePath;
    if(argc > 1)
    {
        filePath = argv[1];
    }else{
        filePath = "./README.md";
    }
    struct stat *buf = new struct stat;
    int ret = lstat(filePath, buf);
    check("stat", ret); 
    PrintStat(buf);
    return 0;
}

