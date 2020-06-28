#include <iostream>
#include "TcpSocket.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage : ./tcp_cli ip port" << std::endl;
        return -1;
    }

    std::string srv_ip = argv[1];
    uint16_t srv_port = std::stoi(argv[2]);

    TcpSocket cli_sock;
    CHECK_RET(cli_sock.SocketCreate());//创建套接字

    CHECK_RET(cli_sock.Connect(srv_ip, srv_port));//发起连接请求

    while (1)
    {
        std::string buf;
        std::cout << "client say :";
        std::cin >> buf;

        CHECK_RET(cli_sock.SendData(buf));

        buf.clear();
        CHECK_RET(cli_sock.RecvData(&buf));

        std::cout << "server say :" << buf << std::endl;
    }

    cli_sock.Close();

    return 0;
}
