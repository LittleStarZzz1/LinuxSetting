#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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

        //int status;

       //wait(&status);//阻塞等待子进程退出
       //waitpid(-1, NULL, 0);//阻塞等待
       while(waitpid(-1, NULL, WNOHANG) == 0);//非阻塞等待
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
