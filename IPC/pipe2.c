/*======================================================
 *通过匿名管道实现管道符的作用
 *实现 ps -ef | grep ssh
 *====================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2] = {-1};
    int ret = pipe(pipefd);
    if (ret < 0)
    {
        perror("pipe error!\n");
        return -1;
    }
    pid_t ps_pid = fork();
    if (ps_pid == 0)
    {
        //ps子进程
        dup2(pipefd[1], 1);
        execlp("ps", "ps", "-ef", NULL);
        exit(0);
    }
    pid_t grep_pid = fork();
    if (grep_pid == 0)
    {
        //grep子进程
        //如果不关闭所有写端, gerp会一直阻塞
        close(pipefd[1]);//关闭写端;ps进程一旦退出,所有的写端被关闭,gerp读完数据后返回0
        dup2(pipefd[0], 0);
        execlp("grep", "grep", "ssh", NULL);
        exit(0);
    }
    //父进程中读写端都有,并且用不到, 因此需要关闭
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(ps_pid, NULL, 0);
    waitpid(grep_pid, NULL, 0);

    return 0;
}
