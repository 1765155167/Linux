#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "hqf_log.h"
#include "sys/wait.h"

int main()
{
    pid_t pid = 0;
    int pidnum = 0;
    const int maxfork = 10000;
    for (; pidnum < maxfork; pidnum++)
    {
        pid = fork();
        check("fork", pid);
        if (pid == 0) {
            break;
        }
    }
    if (pidnum == maxfork) {
        for(int i = 0; i < 100; i++)
            wait(NULL);
    } else {
        execlp("nc","nc","127.0.0.1", "6666", NULL);
    }
    return 0;
}

