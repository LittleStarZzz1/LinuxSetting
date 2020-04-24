#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int g_tickets = 100;
pthread_mutex_t mutex;

void* thr_start(void* arg)
{
   while (1)
   {
       //加锁一定只是保护临界资源
       pthread_mutex_lock(&mutex);
       if (g_tickets > 0)
       {
           //有票就一直抢
           usleep(1000);
           printf("%p-----I get a ticket : %d\n", pthread_self(), g_tickets);
           g_tickets--;
           pthread_mutex_unlock(&mutex);
       }
       else 
       {
           //加锁后在任意有可能退出线程的地方都要解锁
           pthread_mutex_unlock(&mutex);
           pthread_exit(NULL);
       }
   }
   return NULL;
}

int main()
{
    pthread_t tid[4];
    int i, ret;

    //互斥锁的初始化一定要放在线程创建之前
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < 4; ++i)
    {     
        ret = pthread_create(&tid[i], NULL, thr_start, NULL);
        if (ret != 0)
        {
            printf("thread create erroe~~~~\n");
            return -1;
        }
    }

    for (i = 0; i < 4; ++i)
    {
        pthread_join(tid[i], NULL);
    }

    //互斥锁的销毁一定是不再使用这个互斥锁
    pthread_mutex_destroy(&mutex);

    return 0;
}
