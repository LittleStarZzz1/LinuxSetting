#include <vector>
#include <cstdio>
#include <sys/select.h>
#include "TcpSocket.hpp"

#define MAX_TIMEOUT 3000

class Select
{
public:
    Select() :
        _maxfd(-1)
    {
        FD_ZERO(&_rfds);//初始化清空集合
    }

    bool Add(TcpSocket& sock)
    {
        int fd = sock.GetFd();
        FD_SET(fd, &_rfds);
        _maxfd = fd > _maxfd ? fd : _maxfd;//每次修改集合都要重新判断集合中最大描述符
        return true;
    }

    bool Del(TcpSocket& sock)
    {
        int fd = sock.GetFd();
        FD_CLR(fd, &_rfds);//从保存的集合当中移除这个描述符,不再监控这个描述符

        for (int i = _maxfd; i >= 0; --i)
        {
            if (FD_ISSET(i, &_rfds))
            {
                _maxfd = i;
                break;
            }
        }
        return true;
    }

    bool Wait(std::vector<TcpSocket>* list, int outtime = MAX_TIMEOUT)
    {
        struct timeval t_val;
        t_val.tv_sec = outtime / 1000;
        t_val.tv_usec = (outtime % 1000) * 1000;

        fd_set set;
        set = _rfds;//不能直接使用_rfds,因为一旦_rfds被修改就需要外部重新添加描述符了

        int ret = select(_maxfd+1, &set, NULL, NULL, &t_val);
        if (ret < 0)
        {
            perror("select error~~~\n");
            return false;
        }
        else if (ret == 0)
        {
            printf("Waiting timeout~~~\n");
            return true;
        }
        
        for (int i = 0; i < _maxfd+1; ++i)//由于没有额外保存描述符,因此只能从0开始
        {
            if (FD_ISSET(i, &set))//还在集合当中就证明是就绪的
            {
                TcpSocket sock;
                sock.SetFd(i);
                list->push_back(sock);
            }
        }
        return true;
    }//进行监控,并且向外部提供就绪的TcpSocket


private:
    //保存需要监控的可读事件集合的描述符
    //每次监控都是用的是这个集合的复制版(因为select调用返回会清楚未就绪点的描述符)
    //不能让用户每次使用都重新添加描述符,因此使用复制版
    fd_set _rfds;
    //每次监控都需要输入最大描述符
    int _maxfd;
};
