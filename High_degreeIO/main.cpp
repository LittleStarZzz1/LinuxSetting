/***************************************************/
//实现一个多路转接模型的并发服务器
/***************************************************/

#include <iostream>
#include <string>
#include "select.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage : ./main ip port";
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[2]);

    TcpSocket listen_sock;

    //创建监听套接字
    CHECK_RET(listen_sock.SocketCreate());
    
    //绑定服务端地址信息
    CHECK_RET(listen_sock.Bind(ip, port));

    //开始监听
    CHECK_RET(listen_sock.Listen());

    Select s;
    s.Add(listen_sock);//修改事件集合

    while (1)
    {
        std::vector<TcpSocket> list;
        int ret = s.Wait(&list);

        if (ret == false)
            continue;
        for (auto& sock : list)
        {
            if (sock.GetFd() == listen_sock.GetFd())
            {
                //就绪的描述符与监听套接字的描述符一样,表示此时需要获取新连接
                TcpSocket new_sock;
                //从监听套接字获取新建连接
                ret = listen_sock.AcceptConnection(&new_sock);
                if (ret == false)
                    continue;
                s.Add(new_sock);//将新建套接字的描述符也添加到事件集合当中
            }
            //就绪的描述符不是监听套接字的描述符
            //那就是通讯套接字,则进行Recv接收数据
            else
            {
                std::string buf;//接收缓冲区
                ret = sock.RecvData(&buf);
                if (ret == false)
                {
                    sock.Close();//关闭套接字
                    s.Del(sock);//将该套接字的描述符从事件集合中移除
                    continue;
                }
                std::cout << "Client Say :" << buf << std::endl;
                std::cout << "Sever Say :";
                buf.clear();
                std::cin >> buf;

                ret = sock.SendData(buf);
                if (ret == false)
                {
                    sock.Close();
                    s.Del(sock);//关闭套接字则需要移除监控
                }

            }
        }

    }
    listen_sock.Close();
    return 0;
}
