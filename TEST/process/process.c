#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;//定义这个变量只是开辟了一块空间
    pid = fork();//创建子进程

    if (pid == 0)
    {
        printf("i am child~~~~\n");
    }
    else if (pid > 0)
    {
        printf("i am parent~~~~~\n");
    }
    else
    {
        printf("fork errot~~~~~\n");
    }

    while (1)
    {
        sleep(1);
        printf("---------------%d----%d\n", getpid(), pid);
    }





    return 0;
}
