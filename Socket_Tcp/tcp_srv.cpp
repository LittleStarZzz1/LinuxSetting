#include <iostream>
#include "TcpSocket.hpp"

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
        
        //通过监听套接字获取新建连接
        bool ret = listen_sock.AcceptConnection(&new_sock); 
        if (ret == false)
        {
            continue;//服务端不能因为一个新建套接字失败就退出
        }

        std::string tmp;

        //通过新建连接与指定客户端进行通信
        CHECK_RET(new_sock.RecvData(&tmp));
        std::cout << "client say :" << tmp << std::endl;

        tmp.clear();
        std::cout << "server say :";
        std::cin >> tmp;

        CHECK_RET(new_sock.SendData(tmp));
    }

    listen_sock.Close();

    return 0;
}




