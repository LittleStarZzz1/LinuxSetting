#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void func()
{
    //char ptr[] = "This is retval~~~~\n"; ptr有一块空间在栈中, 将字符串的值赋进去
    char* ptr = "This is retval~~~\n"; //字符串在常量区,ptr只是保存这个常量的地址
    pthread_exit((void*)ptr);//线程退出的返回值
}

void* thr_start(void* arg)
{
    //pthread_self(); 返回调用线程的tid
    //pthread_detach(pthread_t tid);
    pthread_detach(pthread_self());//自己分离自己---实际上就是设置一个属性而已

    while (1)
    {
        printf("i am ordinary thread~~~\n");
        sleep(5);
        func();//调用func退出线程
    }

    return NULL;//return 可以退出一个线程
}


int main()
{
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, thr_start, NULL);

    if (ret != 0)
    {
        printf("thread create error~~~\n");
        return -1;
    }

    printf("thread create success~~~\n");


    //char* retval;
    //pthread_join(tid, (void**)&retval);//线程等待,线程返回值为void*, 因此需要二级指针获取返回值
    //printf("retval : %s\n", retval);
    //pthread_cancel(tid);
    while (1)
    {
        printf("main thread~~~\n");
        sleep(1);
    }

    return 0;
}
