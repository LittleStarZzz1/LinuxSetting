#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;// 保护 bowl 变量的访问操作
pthread_cond_t cook_cond;//实现线程间对bowl变量访问的同步操作
pthread_cond_t customer_cond;//实现线程间对bowl变量访问的同步操作

int bowl = 0;//默认表示碗中没有饭

void* thr_cook(void* arg)
{
    while (1)
    {
        //对临界资源访问之间先加锁
        pthread_mutex_lock(&mutex);
        while (bowl != 0)//此时bowl != 0, 表示有饭, 也就是说现在不需要做饭, 
        {
            //让厨师线程等待, 等待之前先解锁, 被唤醒之后再加锁
            //pthread_cond_wait接口中就完成了解锁, 休眠, 被唤醒后加锁三步操作
            //解锁和休眠操作是一步完成, 保证了原子性;
            pthread_cond_wait(&cook_cond, &mutex);

        }
        bowl = 1;//能够走下来表示没饭, bowl==0, 则做一碗饭, 将bowl修改为1
        printf("我做了一碗饭~~~~\n");
        pthread_cond_signal(&customer_cond);//唤醒顾客吃饭
        //解锁
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* thr_customer(void* arg)
{
    while (1)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        while (bowl != 1)//没有饭,不满足吃饭条件,则等待
        {
            //没有饭则等待, 等待之前先解锁, 被唤醒后加锁
            pthread_cond_wait(&customer_cond, &mutex);
        }
        bowl = 0;//能走下来表示有饭, bowl==1, 吃完饭, 将bowl修改为0
        printf("我吃了一碗饭~~~~~\n");
        pthread_cond_signal(&cook_cond);//唤醒厨师做饭
        pthread_mutex_unlock(&mutex);//解锁
    }
    return NULL;
}


int main()
{
    pthread_t cook_tid[4], customer_tid[4];
    int ret;
    int i;
    //在线程创建之前初始化锁
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cook_cond, NULL);
    pthread_cond_init(&customer_cond, NULL);

    for (i = 0; i < 4; ++i)
    {
        ret = pthread_create(&cook_tid[i], NULL, thr_cook, NULL);
        if (ret != 0)
        {
            printf("cook thread create error~~~\n");
            return -1;
        }

        ret = pthread_create(&customer_tid[i], NULL, thr_customer, NULL);

        if (ret != 0)
        {
            printf("customer thread create error~~~\n");
            return -1;
        }
    }

    pthread_join(cook_tid[0], NULL);
    pthread_join(customer_tid[0], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cook_cond);
    pthread_cond_destroy(&customer_cond);

    return 0;
}
