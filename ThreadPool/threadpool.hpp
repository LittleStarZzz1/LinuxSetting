#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <cstdio>
#include <queue>

typedef void (*handler_t)(int);

//封装一个任务类, 定义自己的数据和处理数据的方法
class MyTask
{
public:
    void SetTask(int data, handler_t handler)//设置任务节点
    {
        _data = data;
        _handler = handler;
    }

    void run()//使用给定方法处理给定数据
    {
        _handler(_data);
    }

private:
    int _data;//任务中要处理的数据
    handler_t _handler;//任务中处理数据的方法
};

#define THREAD_MAX 5

class ThreadPool
{
public:
    ThreadPool(int thr_max = THREAD_MAX) :
        _thr_max(thr_max)
    {
        pthread_mutex_init(&_mutex, NULL);//初始化互斥锁
        pthread_cond_init(&_cond, NULL);//初始化条件变量

        int i;
        for (i = 0; i < _thr_max; ++i)
        {
            pthread_t tid;
            int ret = pthread_create(&tid, NULL, thr_start, (void*)this);
            if (ret != 0)
            {
                printf("thread create error~~~~\n");
                exit(-1);//注意析构函数没有返回值
            }
        }
    }


    bool TaskPush(MyTask& t)
    {
        //加锁保护入队的操作
        pthread_mutex_lock(&_mutex);
        _queue.push(t);
        pthread_mutex_unlock(&_mutex);

        pthread_cond_broadcast(&_cond);//入队后唤醒所有线程, 谁抢到谁处理
        return true;
    }

    //此时线程入口函数是成员函数,第一个参数为隐含this指针, 因此定义为static
    static void* thr_start(void* arg)
    {
        ThreadPool* pt = (ThreadPool*)arg;

        while (1)//注意while(1), 线程池中的线程并不是只处理一个任务就退出了
        {
            pthread_mutex_lock(&pt->_mutex);
            while (pt->_queue.empty())
            {
                pthread_cond_wait(&pt->_cond, &pt->_mutex);
            }

            MyTask mt;
            mt = pt->_queue.front();
            pt->_queue.pop();
            pthread_mutex_unlock(&pt->_mutex);
            mt.run();
            //任务的处理要放在解锁之外, 因为当前保护的是对队列的操作
            //否则下一个任务的处理只能等到解锁
        }

        return NULL;
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

private:
    int _thr_max;//线程池中线程的最大数量, 根据这个变量初始化创建指定数量的线程

    std::queue<MyTask> _queue;//stl中提供的queue为了保证性能并没有线程安全
    pthread_mutex_t _mutex;//实现队列的互斥操作, 即保证临界资源访问的安全性
    pthread_cond_t _cond;//实现队列的同步操作, 即保证临界资源访问的合理性
};








