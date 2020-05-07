#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <semaphore.h>
#include <vector>

#define MAX_QUEUE 5//最大节点数量

class RingQueue
{
public:
    RingQueue(int maxq = MAX_QUEUE) :
        _queue(maxq),
        _capacity(maxq),
        _step_write(0),
        _step_read(0)
    {
        //sem_init(信号量, 线程(0)/进程标志(非0), 信号量初值);
        sem_init(&_lock, 0, 1);//用于实现互斥, 即为1的计数器

        sem_init(&_idle_sem, 0, maxq);//空闲空间计数初始为节点个数
        sem_init(&_data_sem, 0, 0);//数据空间计数初始为0
    }

    ~RingQueue()
    {
        sem_destroy(&_lock);

        sem_destroy(&_idle_sem);
        sem_destroy(&_data_sem);
    }

    bool Push(int val)
    {
        //要插入数据首先看空闲空间计数是否大于0, 大于0则访问, 否则阻塞
        sem_wait(&_idle_sem);//空间空间计数的判断, 能访问空闲空间计数-1
        
        //可以访问则加锁
        sem_wait(&_lock);//_lock的计数不大于1, 当前若可以访问则-1, 别人就不能访问
        _queue[_step_write] = val;
        _step_write = (_step_write + 1) % _capacity;
        sem_post(&_lock);//访问完毕解锁, 唤醒其他因加锁阻塞的线程

        sem_post(&_data_sem);//插入数据, 已有数据空间计数+1, 唤醒消费者
    }

    bool Pop(int* data)
    {
        //要pop数据首先要看已有数据空间计数是否大于0, 大于0则可以访问, 否则阻塞
        sem_wait(&_data_sem);//对已有数据的空间进行计数, 能访问则已有数据空间计数-1

        //能访问则加锁
        sem_wait(&_lock);//_lock计数不大于1, 能访问则计数-1, 别人就不能访问
        *data = _queue[_step_read];
        _step_read = (_step_read + 1) % _capacity;
        sem_post(&_lock);//解锁

        sem_post(&_idle_sem);//取出数据,则空闲空间+1, 唤醒生产者

    }

private:
    std::vector<int> _queue;//数组, vector需要初始化节点数量
    int _capacity;//队列节点数量
    int _step_write;//写入数据的位置下标
    int _step_read;//获取数据的位置下标

    sem_t _lock;//信号量实现互斥

    //这个信号量是对空闲空间进行计数的
    //对于生产者来说, 当空闲空间计数大于0的时候才能写数据, 初始为最大节点个数
    sem_t _idle_sem;//空闲空间资源计数

    //这个信号量是用于对已有数据的空间进行计数
    //对于消费者来说有数据的空间计数大于0的时候才能取出数据, 初始为0
    sem_t _data_sem;//已有数据空间资源数量
};

void* thr_pro(void* arg)
{
    RingQueue* p_queue = (RingQueue*) arg;
    int i = 0;

    while (1)
    {
        p_queue->Push(i);

        printf("produed a data : %d\n", i++);
    }
    return NULL;
}

void* thr_cus(void* arg)
{
    RingQueue* q_queue = (RingQueue*)arg;

    while (1)
    {
        int data;
        q_queue->Pop(&data);

        printf("pop a data : %d~~~~\n", data);
    }
    return NULL;
}

int main()
{
    pthread_t pro_tid[4], cus_tid[4];

    int i, ret;
    RingQueue queue;

    for (i = 0; i < 4; ++i)
    {
        ret = pthread_create(&pro_tid[i], NULL, thr_pro, (void*)&queue);
        if (ret != 0)
        {
            printf("productor thread create error~~~~\n");
            return -1;
        }

        ret = pthread_create(&cus_tid[i], NULL, thr_cus, (void*)&queue);
        if (ret != 0)
        {
            printf("customer thread create error~~~~\n");
            return -1;
        }
    }

    for (i = 0; i < 4; ++i)
    {
        pthread_join(pro_tid[i], NULL);
        pthread_join(cus_tid[i], NULL);
    }


    return 0;
}
