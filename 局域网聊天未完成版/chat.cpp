#include "chat.h"


//初始化机器名和ip列表
Chat::Chat(QObject *parent) : QObject(parent)
{
    name = getSysName();
    ips = getSysIps();

#ifdef WIN32
    //如果是windows环境下，初始化socket运行环境
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,1), &wsaData);
#endif
}

void *Chat::recv_thread(void *ptr)
{

}

/*
    1.创建通信socket
    2.绑定ip地址和端口
    3.发送上线广播报文
    4.创建子线程进行信息交互
*/
void Chat::init()
{
    this->udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(this->udp_fd < 0)
    {
        qDebug() << "error create socket";
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(udp_fd, (sockaddr *)&addr, sizeof(addr));
    if(ret != 0)
    {
        qDebug() << "error bind";
        exit(1);
    }

    //设置该socket， 可以发送广播
    int arg = 1;
#ifdef WIN32
    setsockopt(udp_fd, SOL_SOCKET, SO_BROADCAST, (char *)&arg, sizeof(arg));
#else
    setsockopt(udp_fd, SOL_SOCKET, SO_BROADCAST, &arg, sizeof(arg));
#endif
    //发送上线的广播
    /*
        {
            cmd: online
            name: account-name
        }
    */
    QJsonObject obj;
    obj.insert(CMD, ONLINE);
    obj.insert(NAME, name);

    send(obj, "192.168.19.255");

    //创建接收线程
    pthread_create(&tid, NULL, recv_thread, this);
}

QString Chat::getSysName()
{

}

QStringList Chat::getSysIps()
{

}

void Chat::send(const QJsonObject &obj, QString ip)
{

}

void Chat::sendMsg(QString content, QString ip, bool boardcast)
{

}
