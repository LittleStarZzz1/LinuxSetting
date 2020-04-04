#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    //匿名管道只能用于具有亲缘关系的进程间通信, 子进程复制父进程的方式获取操作句柄
    //注意:创建管道一定要放在创建子进程之前
    int pipefd[2] = { -1 };
    int ret = pipe(pipefd);
    if (ret < 0)
    {
        perror("pipe error!\n");
        return -1;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        //close(pipefd[1]);//关闭所有写端
        //sleep(3);
        //子进程用来从管道中读取数据
        char buf[1024] = { 0 };
        int rlen = read(pipefd[0], buf, 1023);//从管道中读取数据用pipefd[0]
        printf("read sucessful:%d-%s\n", rlen, buf);
    }
    else if (pid > 0)
    {
        //close(pipefd[1]);
        //父进程用来向管道中写入数据
        //sleep(1000);//为了让子进程先运行
        char *data = "今天天气好冷啊~~~~\n";
        int total_len = 0;
        while (1)
        {
            //写入数据是一个循环的过程, 不断写入
            int wlen = write(pipefd[1], data, strlen(data));//向管道中写入数据用pipefd[1], 返回成功写入的字节数
            total_len += wlen;
            printf("total_len = %d\n", total_len);
        }
    }
    return 0;
}
