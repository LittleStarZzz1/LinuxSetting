/*=====================================================
 * 这个demo演示修改信号的处理方式; 通过这个demo体会修改的过程
 * signal(int signo, sighandler_t handler);
 * ==================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigcb(int signo)
{
   printf("recv a signal number:%d\n", signo); 
}

int main()
{
    //当信号值到来的时候才会调用sigcb这个函数, 并且通过参数传入触发回调函数的信号值
    signal(SIGINT, sigcb);//这一步只是修改内核中信号的回调函数指针
    //signal(SIGQUIT, sigcb);

    while (1)
    {
        printf("希望眼前的人一切安好~~~!\n");
        sleep(1);
    }

    return 0;
}
