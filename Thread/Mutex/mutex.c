#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
int g_tickets = 100;

void *thr_tout(void * arg)
{
    while (1)
    {
        //加锁一定是在访问临界资源之前加锁
        //pthread_mutex_lock 阻塞加锁
        //pthread_mutex_trylock 非阻塞加锁
        //pthread_mutex_timedlock 限时等待的阻塞加锁
        pthread_mutex_lock(&mutex);
        if (g_tickets > 0)
        {
            usleep(100);
            printf("tout:%p - get a ticket:%d\n", pthread_self(), g_tickets);
            g_tickets--;
            //解锁一定是在访问临界资源之后
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            //注意加锁之后在任意有可能退出线程的地方都要解锁
            printf("tout:%p exit\n", pthread_self());
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
    }
    return NULL;
}

int main()
{
    int i = 0;
    int ret;
    pthread_t tid[4];
    
    //互斥锁的初始化必须放在创建线程之前
    //pthread_mutex_t_init();
    //pthread_mutex_t mutex = PTHREAD_MUTEX_INITALIZER;
    pthread_mutex_init(&mutex, NULL);
    for (; i < 4; ++i)
    {
        ret = pthread_create(&tid[i], NULL, thr_tout, NULL);
        if (ret != 0)
        {
            printf("pthread create error!\n");
            return -1;
        }
    }
    for (i = 0; i < 4; ++i)
    {
        pthread_join(tid[i], NULL);
    }
    //切记互斥锁的销毁
    pthread_mutex_destroy(&mutex);

    return 0;
}
