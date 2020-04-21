#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void* thread_start(void* arg)
{
    while (1)
    {
        sleep(1);
        printf("i am ordinary thread : %s\n", arg);
    }
}

int main()
{
    pthread_t tid;
    char *buf = "hello world~~~";
    int ret = pthread_create(&tid, NULL, thread_start, (void*)buf);
    if (ret != 0)
    {
        printf("thread create error!\n");
        return -1;
    }

    printf("thread create success~~~~\n");

    while (1)
    {
        sleep(1);
        printf("main thread : 你吃了吗~~~~~!\n");
    }

    while (1)
    {
        sleep(1);
        printf("今天天气好冷~~~~~\n");
    }

    return 0;
}
