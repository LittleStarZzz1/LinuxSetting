#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    while (1)
    {
        //增加一个shell提示
        printf("[user@minishell]$ ");
        fflush(stdout);
        //1.等待标准输入
        char buf[1024] = { 0 };
        fgets(buf, 1023, stdin);//从标准输入获取数据
        buf[strlen(buf) - 1] = '\0';//防止将换行符\n放进buf中+
        //2.对输入数据进行解析
        int argc = 0;
        char *argv[32] = { NULL };
        char *ptr = buf;
        while (*ptr != '\0')
        {
            if (*ptr != ' ')
            {
                argv[argc] = ptr;
                argc++;
                while (*ptr != ' ' && *ptr != '\0')
                {
                    ptr++;
                }
                *ptr = '\0';
            }
            ptr++;
        }
        argv[argc] = NULL;//最后一个参数的下一个位置置NULL
        //3.创建子进程
        pid_t pid = fork();
        //4.在子进程当中进行程序替换
        if (pid == 0)
        {
            int ret =  execvp(argv[0], argv);
            if (ret < 0)
            {
                perror("execv error");
                return -1;
            }
            exit(0);
        }
        //5.进程等待
        wait(NULL);
    }
    return 0;
}
