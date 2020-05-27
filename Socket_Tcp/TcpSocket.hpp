#include <cstdio>
#include <string>
#include <unistd.h>
#include <sys/socket.h>//套接字接口
#include <netinet/in.h>//包含地址结构信息的头文件
#include <arpa/inet.h>//字节序转换

#define MAX_LISTEN 5
#define CHECK_RET(q) if((q)==false){return -1;}


class TcpSocket
{
public:
    TcpSocket() :
        _sockfd(-1)
    {

    }

    //创建套接字
    bool SocketCreate()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_sockfd < 0)
        {
            perror("socket create error~~~\n");
            return false;
        }

        return true;
    }
    
    //绑定地址结构
    bool Bind(const std::string& ip, const uint16_t port)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);//主机字节序转网络字节序

        //主机点分十进制ip地址转网络整数ip地址
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

    //开始监听
    bool Listen(int backlog = MAX_LISTEN)
    {
        int ret = listen(_sockfd, backlog);

        if (ret < 0)
        {
            perror("listen error~~~\n");
            return false;
        }

        return true;
    }

    //获取新建连接
    bool AcceptConnection(TcpSocket* newsock,std::string* ip=NULL,uint16_t* port=NULL)
    {
        struct sockaddr_in addr;

        socklen_t len = sizeof(struct sockaddr_in);

        int new_sockfd = accept(_sockfd, (struct sockaddr*)&addr, &len);
        if (new_sockfd < 0)
        {
            perror("accept error~~~\n");
            return false;
        }

        newsock->_sockfd = new_sockfd;

        if (ip != NULL)
        {
            *ip = inet_ntoa(addr.sin_addr);
        }

        if (port != NULL)
        {
            *port = ntohs(addr.sin_port);
        }
        return true;
    }

    //接收数据
    bool RecvData(std::string* buf)
    {
        char tmp[4096] = { 0 };
        int ret = recv(_sockfd, tmp, 4096, 0);
        if (ret < 0)
        {
            perror("recv error~~~\n");
            return false;
        }
        else if (ret == 0)
        {
            //recv默认阻塞,没有数据就会等待,返回0表示连接断开
            printf("connection broken~~~\n");
            return false;
        }
        buf->assign(tmp, ret);

        return true;
    }

    //发送数据
    bool SendData(const std::string& data)
    {
        int ret = send(_sockfd, data.c_str(), data.size(), 0);

        if (ret < 0)
        {
            perror("send error~~\n");
            return false;
        }

        return true;
    }

    //建立连接(客户端向服务端发起连接请求)
    bool Connect(const std::string& ip, const uint16_t port)
    {
       struct sockaddr_in addr;
       addr.sin_family = AF_INET;
       addr.sin_port = htons(port);
       addr.sin_addr.s_addr = inet_addr(ip.c_str());

       socklen_t len = sizeof(struct sockaddr_in);

       int ret = connect(_sockfd, (struct sockaddr*)&addr, len);

       if (ret < 0)
       {
           perror("connect error~~~\n");
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

