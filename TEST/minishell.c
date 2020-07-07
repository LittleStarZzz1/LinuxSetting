#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    while (1)
    {
        printf("[minishell@] : ");
        fflush(stdout);

        int argc = 0;
        char* argv[32] = { NULL };

        char buf[1024] = { 0 };
        fgets(buf, 1023, stdin);//从标准输入读取数据
        buf[strlen(buf) - 1] = '\0';//将最后获取到的换行符置为'\0'
        printf("%s\n", buf);

        //字符串解析
        char* ptr = buf;
        while (*ptr != '\0')
        {
            if (*ptr != ' ')
            {
                argv[argc] = ptr;
                argc++;
                while (*ptr != ' ' && *ptr != '\0')
                    ++ptr;
                *ptr = '\0';
            }
            ptr++;
        }
        argv[argc] = NULL;

        pid_t pid = fork();//创建子进程
        if (pid < 0)
        {
            perror("fork error~~~\n");
            continue;
        }
        else if (pid == 0)
        {
            execvp(argv[0], argv);//程序替换
            exit(0);
        }

        wait(NULL);//等待子进程退出,获取子进程返回值,释放子进程资源

    }


    return 0;
}
