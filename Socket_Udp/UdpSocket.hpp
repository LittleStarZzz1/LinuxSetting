#include <cstdio>
#include <unistd.h>
#include <string>
#include <sys/socket.h>//套接字接口信息
#include <arpa/inet.h>//字节序转换的我文件
#include <netinet/in.h>//包含地址结构信息的头文件

#define CHECK_RET(q) if((q) == false){return false;}

class Udp_Socket
{
public:
    Udp_Socket() :
        _sockfd(-1)
    {

    }

    //创建套接字
    bool SocketCreate()
    {
        //socket(地址域, 套接字类型, 协议类型)
        _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (_sockfd < 0)
        {
            perror("socket create error~~~\n");
            return false;
        }
        return true;
    }

    //绑定地址信息
    bool Bind(const std::string& ip, uint16_t port)
    {
        struct sockaddr_in addr;//定义ipv4地址结构
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);//主机字节序短整型数据转网络字节序
        //点分十进制字符串ip地址转网络整数ip地址
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(struct sockaddr_in);

        int ret = bind(_sockfd, (struct sockaddr*)&addr, len);

        if (ret < 0)
        {
            perror("bind error~~~\n");
            return false;
        }

        return true;
    }

    //接收数据
    bool RecvData(std::string* buf, std::string* ip = NULL, uint16_t* port = NULL)
    {
        struct sockaddr_in peer_addr;
        socklen_t len = sizeof(struct sockaddr_in);
        
        char tmp[4096] = { 0 };

        int ret = recvfrom(_sockfd, tmp, 4096, 0, (struct sockaddr*)&peer_addr, &len);

        if (ret < 0)
        {
            perror("recvfrom error~~~\n");
            return false;
        }
        
        buf->assign(tmp, ret);//assign从指定字符串中截取ret长度的数据到buf中

        if (ip != NULL)
        {
            //网络整型ip地址转主机点分十进制ip地址
            *ip = inet_ntoa(peer_addr.sin_addr);
        }

        if (port != NULL)
        {
            *port = ntohs(peer_addr.sin_port);//网络字节序转主机字节序
        }

        return true;
    }

    //发送数据
    bool SendData(const std::string& data, std::string& ip, const uint16_t port)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());

        socklen_t len = sizeof(struct sockaddr_in);

        int ret = sendto(_sockfd, data.c_str(), data.size(), 0, 
                (struct sockaddr*)&addr, len);

        if (ret < 0)
        {
            perror("sendto error~~~~\n");
            return false;
        }

        return true;
    }

    //关闭套接字
    bool Close()
    {
        if (_sockfd > 0)
        {
            close(_sockfd);
            _sockfd = -1;
        }
        return true;
    }

private:
    int _sockfd;
};

