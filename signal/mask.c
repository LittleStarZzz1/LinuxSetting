#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigcb(int signo)
{
    printf("recv signo:%d\n", signo);
}

int main()
{
    signal(SIGINT, sigcb);
    signal(SIGRTMIN+4, sigcb);

    sigset_t set;

    sigemptyset(&set);//清空set集合
    sigfillset(&set);//将所有信号添加到set集合当中
    sigprocmask(SIG_BLOCK, &set, NULL);//阻塞set集合当中的信号

    printf("wait enter continue~~~~!\n");
    getchar();//等待一个回车, 如果不按回车就一直卡在这里;

    sigprocmask(SIG_UNBLOCK, &set, NULL);//解除set集合当中信号的阻塞

    while (1)
    {
        sleep(1);
    }

    return 0;
}
