#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>
#include <pthread.h>

#define MAX_QUEUE_SIZE 5

class BlockQueue
{
public:
    BlockQueue(int maxq = MAX_QUEUE_SIZE) :
        _capacity(maxq)
    {
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_pro_cond, NULL);
        pthread_cond_init(&_cus_cond, NULL);
    }

    bool PushData(int data)
    {
        pthread_mutex_lock(&_mutex);

        while (_queue.size() == _capacity)
        {
            pthread_cond_wait(&_pro_cond, &_mutex);
        }
        _queue.push(data);
        pthread_mutex_unlock(&_mutex);

        //唤醒消费者
        pthread_cond_signal(&_cus_cond);
        return true;
    }
    bool PopData(int *data)
    {
        pthread_mutex_lock(&_mutex);

        while (_queue.empty())
        {
            pthread_cond_wait(&_cus_cond, &_mutex);
        }
        *data = _queue.front();//取出队首数据
        _queue.pop();
        pthread_mutex_unlock(&_mutex);

        pthread_cond_signal(&_pro_cond);//唤醒生产者
        return true;
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_pro_cond);
        pthread_cond_destroy(&_cus_cond);
    }

private:
    std::queue<int> _queue;
    int _capacity;
    //int _num;

    pthread_mutex_t _mutex;//实现互斥
    pthread_cond_t _pro_cond;
    pthread_cond_t _cus_cond;
};

void* thr_pro(void* arg)
{
    BlockQueue *q = (BlockQueue*)arg;
    int i = 0;

    while (1)
    {
        q->PushData(i);
        printf("i push a data : %d\n", i++);
    }
    return NULL;
}

void* thr_cus(void* arg)
{
    BlockQueue *q  = (BlockQueue*)arg;
    while (1)
    {
        int data;
        q->PopData(&data);

        printf("i get a data : %d\n", data);
    }
    return NULL;
}

int main()
{
    pthread_t ptid[4], ctid[4];

    BlockQueue q;

    int i;
    for (i = 0; i < 4; ++i)
    {
        int ret = pthread_create(&ptid[i], NULL, thr_pro, (void*)&q);
        if (ret != 0)
        {
            printf("create thread productor error~~\n");
            return -1;
        }
        ret = pthread_create(&ctid[i], NULL, thr_cus, (void*)&q);
        if (ret != 0)
        {
            printf("create customer thread error~~\n");
            return -1;
        }
    }

    for (i = 0; i < 4; ++i)
    {
        pthread_join(ptid[i], NULL);
        pthread_join(ctid[i], NULL);
    }
    return 0;
}


