#include <unistd.h>
#include "threadpool.hpp"

void func1(int data)
{
    int sec = (data % 3) + 1;
    printf("tid : %p-func1 : i get a data : %d\n", pthread_self(), data);
    sleep(sec);
}

void func2(int data)
{
    printf("tid : %p-func2 : i am running : %d\n", pthread_self(), data);
    sleep(1);
}

int main()
{
    ThreadPool pool;//定义一个线程池
    
    int i;
    for (i = 0; i < 10; ++i)
    {
        MyTask task;
        if (i % 2 == 0)
        {
            task.SetTask(i, func1);
        }
        else 
        {
            task.SetTask(i, func2);
        }
        pool.TaskPush(task);
    }

    sleep(1000);

    return 0;
}
