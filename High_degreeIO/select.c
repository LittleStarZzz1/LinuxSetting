#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char* argv[])
{
    //通过对标准输入的监控, 体会监控的作用
    while (1)
    {
        printf("Start monitoring\n");
        fd_set set;//每次监控都需要重新添加描述符
        //调用select最终会清空集合中未就绪的描述符
        FD_ZERO(&set);//初始化清空集合
        FD_SET(0, &set);//将标准输入的描述符添加到事件集合当中
        
        struct timeval t_val;
        t_val.tv_sec = 3;
        t_val.tv_usec = 3;;

        //select(最大描述符加1, 可读事件集合, 可写事件集合, 异常事件集合, 超时等待时间)
        int ret = select(0+1, &set, NULL, NULL, &t_val);
        if (ret < 0)
        {
            perror("select error~~~\n");
            return -1;
        }
        else if (ret == 0)
        {
            printf("Monitoring timeout\n~~~~");
            continue;//没有描述符就绪的情况下返回就是超时了
        }
        printf("Descriptor ready or timeout waiting~~~~\n");

        if (FD_ISSET(0, &set))
        {
            printf("Start reading data~~~~\n");
            char buf[1024] = {0};
            ret = read(0, buf, 1023);
            if (ret < 0)
            {
                perror("read error~~~~\n");
                return -1;
            }
            printf("%s\n", buf);
        }
    }

    return 0;
}

