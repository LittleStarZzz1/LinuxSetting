#include <iostream>
#include "UdpSocket.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage : ./udp_cli ip port" << std::endl;
        return -1;
    }

    std::string srv_ip = argv[1];
    uint16_t srv_port = std::stoi(argv[2]);

    Udp_Socket cli_sock;

    CHECK_RET(cli_sock.SocketCreate());//创建套接字

    while (1)
    {
        std::string data;
        std::cout << "client say :";
        std::cin >> data;

        CHECK_RET(cli_sock.SendData(data, srv_ip, srv_port));

        data.clear();

        CHECK_RET(cli_sock.RecvData(&data));

        std::cout << "server say :" << data << std::endl;
    }

    cli_sock.Close();//关闭套接字

    return 0;
}
