#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main()
{
    //kill(进程id, 信号值);
    //kill(getpid(), SIGINT);//给指定的进程发送指定的信号
    //kill(getpid(), SIGHUP);
    //raise(SIGTERM);//给进程自己发送指定的信号
    //abort();//给自己发送SIGABRT信号, 通常用于异常通知
    alarm(3);//3秒之后给进程自己发送SIGALRM信号

    while (1)
    {
        printf("今天天气好冷呀~~!\n");
        sleep(1);
    }

    return 0;
}
