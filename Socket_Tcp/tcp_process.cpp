#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include "TcpSocket.hpp"

void sigcb(int no)
{
    //SIGCHILD是一个非可靠信号,有可能丢失
    //因此在一次信号处理当中,就需要处理到没有子进程退出为止
    while (waitpid(-1, NULL, WNOHANG) > 0);//返回值大于表示有子进程退出
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage ./tcp_srv ip port" << std::endl;
        return -1;
    }

    signal(SIGCHLD, sigcb);

    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[2]);

    TcpSocket listen_sock;

    CHECK_RET(listen_sock.SocketCreate());//创建套接字
    CHECK_RET(listen_sock.Bind(ip, port));//绑定地址信息
    CHECK_RET(listen_sock.Listen());//开始监听

    while (1)
    {
        TcpSocket new_sock;
        
        //通过监听套接字获取新建连接
        bool ret = listen_sock.AcceptConnection(&new_sock); 
        if (ret == false)
        {
            continue;//服务端不能因为一个新建套接字失败就退出
        }
        
        int pid = fork();//创建子进程,子进程复制父进程,父进程有的子进程都有

        if (pid == 0)
        {
            while (1)
            {
                std::string tmp;

                //通过新建连接与指定客户端进行通信
                CHECK_RET(new_sock.RecvData(&tmp));
                std::cout << "client say :" << tmp << std::endl;

                tmp.clear();
                std::cout << "server say :";
                std::cin >> tmp;

                CHECK_RET(new_sock.SendData(tmp));
            }
            new_sock.Close();
            exit(0);
        }
        new_sock.Close();

    }

    listen_sock.Close();

    return 0;
}




