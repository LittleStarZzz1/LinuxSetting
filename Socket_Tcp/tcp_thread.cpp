#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include "TcpSocket.hpp"

void* thr_worker(void* arg)
{
    long fd = (long)arg;
    TcpSocket new_sock;
    new_sock.SetFd(fd);
    while (1)
    {
        std::string buf;
        new_sock.RecvData(&buf);
        std::cout << "client say : " << buf << std::endl;

        buf.clear();
        std::cout << "server say : ";
        std::cin >> buf;
        new_sock.SendData(buf);
    }
    new_sock.Close();//线程之间文件描述符共享,这边关闭了描述符其他地方就用不了了
    return NULL;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage ./tcp_srv ip port" << std::endl;
        return -1;
    }


    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[2]);

    TcpSocket listen_sock;

    CHECK_RET(listen_sock.SocketCreate());//创建套接字
    CHECK_RET(listen_sock.Bind(ip, port));//绑定地址信息
    CHECK_RET(listen_sock.Listen());//开始监听

    while (1)
    {
        TcpSocket new_sock;
        //TcpSocket* new_sock = new TcpSocket();//堆上创建TcpSocket对象

        //通过监听套接字获取新建连接
        bool ret = listen_sock.AcceptConnection(&new_sock); 
        if (ret == false)
        {
            continue;//服务端不能因为一个新建套接字失败就退出
        }
        
        pthread_t tid;
        //直接传值
        //new_sock是一个局部对象,循环完毕会被释放,传局部变量的地址十分危险,会造成错误
        //除非每次accept的时候new_sock在堆上申请一个新的,不会自动释放
        //也要防止第二次获取的时候覆盖上次的值
        //或者直接传值在函数栈中会新建空间存储与原数据空间就没有什么关系了-仅限于数据空间较小的时候
        int res = pthread_create(&tid, NULL, thr_worker, (void*)new_sock.GetFd());
        if (res != 0)
        {
            std::cout << "pthread create error~~" << std::endl;
            continue;
        }

        pthread_detach(tid);//不关心线程返回值,也不想等待释放资源,因此分离线程

        //主线程不能关闭套接字,因为线程间描述符是共享的.

    }

    listen_sock.Close();

    return 0;
}




