#include <iostream>
#include "UdpSocket.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: ./udp_srv ip port" << std::endl;
        return -1;
    }

    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[2]);

    Udp_Socket srv_sock;

    CHECK_RET(srv_sock.SocketCreate());//创建套接字
    CHECK_RET(srv_sock.Bind(ip, port));//绑定地址结构

    while (1)
    {
        std::string buf;//缓冲区
        std::string peer_ip;
        uint16_t peer_port;

        //接收数据以及对端地址信息
        CHECK_RET(srv_sock.RecvData(&buf, &peer_ip, &peer_port));

        std::cout << "client say :" << buf << std::endl;

        buf.clear();
        std::cout << "server say :";
        std::cin >> buf;

        //谁发送了数据就给谁回复数据
        CHECK_RET(srv_sock.SendData(buf.c_str(), peer_ip, peer_port));

    }

    //关闭套接字
    srv_sock.Close();

    return 0;
}
