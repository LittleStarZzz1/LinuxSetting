#include <iostream>
#include <cstdio>
#include <queue>
#include <pthread.h>

#define MAX_QUEUE 5

//封装线程安全的队列, 对外提供入队/出队操作
class BlockQueue
{
public:
    BlockQueue(int maxq = MAX_QUEUE) :
        _capacity(maxq)
    {
        pthread_mutex_init(&_mutex, NULL);

        pthread_cond_init(&_pro_cond, NULL);
        pthread_cond_init(&_cus_cond, NULL);

    }

    //入队
    bool Push(const int& val)
    {
        //生产者才会入队数据, 如果队列中数据满了则需要阻塞
        pthread_mutex_lock(&_mutex);//访问临界资源之前加锁
        while (_queue.size() == _capacity)
        {
           // 队列中数据已满, 此时解锁, 生产者线程挂起等待
           pthread_cond_wait(&_pro_cond, &_mutex);//被唤醒后加锁
        }
        _queue.push(val);
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_cus_cond);
        return true;
    }

    //出队
    bool Pop(int* data)//输出型参数
    {
        //只有消费者才会出队数据, 有数据才能出队, 没有数据则需要阻塞
        pthread_mutex_lock(&_mutex);//访问临界资源之前加锁
        while (_queue.empty())
        {
            pthread_cond_wait(&_cus_cond, &_mutex);
        }
        *data = _queue.front();//获取队列中首个节点
        _queue.pop();//出队
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_pro_cond);
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

    pthread_mutex_t _mutex;
    pthread_cond_t _pro_cond;
    pthread_cond_t _cus_cond;

};

void* thr_pro(void* arg)
{
    BlockQueue* q = (BlockQueue*)arg;
    int i = 0;
    while (1)//生产者不断生产数据;
    {
        q->Push(i);
        printf("Push Data : %d\n", i++);
    }
    return NULL;
}

void* thr_cus(void* arg)
{
    BlockQueue* q = (BlockQueue*)arg;
    while (1)
    {
        int ret;
        q->Pop(&ret);
        printf("Pop Data : %d\n", ret);
    }
    return NULL;
}

int main()
{
    pthread_t pro_tid[4], cus_tid[4];

    BlockQueue q;
    int i;
    int ret;
    for (i = 0; i < 4; ++i)
    {
        ret = pthread_create(&pro_tid[i], NULL, thr_pro, (void*)&q);
        if (ret != 0)
        {
            printf("pro thread create error~~~\n");
            return -1;
        }
        ret = pthread_create(&cus_tid[i], NULL, thr_cus, (void*)&q);
        if (ret != 0)
        {
            printf("cus`` thread create error~~~\n");
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
