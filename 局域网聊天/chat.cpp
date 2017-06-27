#include "chat.h"
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QProcessEnvironment>
#include <unistd.h>
#include <QThread>

//初始化机器名和ip列表
Chat::Chat(QObject *parent) : QObject(parent)
{
    account = getSysName();
    ips = getSysIps();
    broadcast_ip = "255.255.255.255";
    udp_fd = -1;

#ifdef WIN32
    //如果是windows环境下，初始化socket运行环境
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,1), &wsaData);
#endif
}

//接收线程获取本对象指针
//执行run函数
void *Chat::recv_thread(void *ptr)
{
    Chat *This = (Chat *)ptr;

    This->run();
    return NULL;
}

//循环接收其它客户端发送的消息
//调用其它函数解析出来消息
void Chat::run()
{
    while(1)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        char buf[4096];
        //接收数据
        int ret = recvfrom(udp_fd, buf, sizeof(buf), 0, (struct sockaddr*)&addr, &len);
        if(ret > 0)
        {
            buf[len] = 0;   //对方没有发送\0，所以自己加
            QJsonDocument doc = QJsonDocument::fromJson(QByteArray(buf));   //将读到的数据转化为Json格式的数据

            //char ip[16];
            //inet_ntoa_r(addr.sin_addr, ip, sizeof(ip));

            //将处理完的数据对象和ip地址作为参数传入；进行数据回应处理
            handleMsg(doc.object(), inet_ntoa(addr.sin_addr));
        }
        else
        {
           QThread::msleep(100);
        }
    }
}

//运行在独立的线程上下文， Qt有规定，线程不能更新界面
void Chat::handleMsg(const QJsonObject &obj, QString ip)
{
    if(ips.indexOf(ip) != -1)   //判断自己的ip列表里是否有形参ip
    {
        qDebug() << "自己发送数据给自己，丢弃";
        return;
    }

    //通过QJson的key -->CMD;判断是接收到的是哪种报文消息
    QString cmd = obj.value(CMD).toString();
    if(cmd == ONLINE)   //上线广播消息处理
    {
        QString name = obj.value(NAME).toString();

        addUser(ip, name);  //将上线的用户添加到自己的用来列表；并通知主界面更新窗口信息

        //回应这个用户
        QJsonObject resp;
        resp.insert(CMD, ONLINEACK);    //在线用户应答报文
        resp.insert(NAME, name);
        send(resp, ip); //给新上线用户发送应答报文
    }
    if(cmd == ONLINEACK)    //上线应答消息处理
    {
        //添加已上线的用户到自己用户列表
        QString name = obj.value(NAME).toString();
        addUser(ip, name);
    }
    if(cmd == CHAT) //聊天消息处理
    {
        bool broadcast = obj.value(BROADCAST1).toBool();   //是否是群发消息
        QString content = obj.value(CONTENT).toString();    //单用户一对一发送消息
        QString name = obj.value(NAME).toString();  //获取发送者的名字

        emit this->sigNewContent(name, content, broadcast); //发送信号给主界面更新
    }
}


//添加新上线用户到others列表中
//发送信号给主界面，让主界面更新
void Chat::addUser(QString ip, QString name)
{
    User *user = new User;
    user->ip = ip;
    user->name = name;

    if(others[ip])  //如果others列表中有此用户的信息，直接删除
        delete others[ip];
    //新上线用户入表
    others[ip] = user;

    //发送信号；通知界面更新
    emit sigNewUser(name, ip);
}

/*
    1.创建通信socket
    2.绑定ip地址和端口
    3.发送上线广播报文
    4.创建子线程进行信息交互
*/
void Chat::init()
{
    create_socket("0.0.0.0");

    sendOnline();

    //qDebug() << "开始发送上线广播";
    //创建接收线程
    pthread_create(&tid, NULL, recv_thread, this);
}

QString Chat::getSysName()
{
#ifdef WIN32
    return QProcessEnvironment::systemEnvironment().value("USERNAME");
#else
    char buf[1024];
    memset(buf,0, sizeof(buf));
    FILE* fp = popen("whoami", "r");
    int ret = fread(buf, 1, sizeof(buf), fp);
    buf[ret-1] = 0;
    fclose(fp);
    return QString(buf);
#endif
}

QStringList Chat::getSysIps()
{
    QStringList ret;
    QList<QNetworkAddressEntry> entrys;

    QList<QNetworkInterface> infs = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface inf, infs)
    {
        entrys.append(inf.addressEntries());
    }
    foreach(QNetworkAddressEntry entry, entrys)
    {
        if(entry.ip().toString().length() != 0)
            ret.append(entry.ip().toString());

#if 0
        if(entry.broadcast().toString().isEmpty())
            continue;
        qWarning() << "ip and broadcast ip:"
                   << entry.ip().toString()
                   << entry.broadcast().toString();
        items[ITEM_SHARE_ADDR]->addItem(entry.broadcast().toString());
#endif
    }
    return ret;
}

void Chat::create_socket(QString ip)
{
    if(this->udp_fd != -1)
        close(this->udp_fd);

    this->udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(this->udp_fd < 0)
    {
        qDebug() << "error create socket";
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = inet_addr(ip.toUtf8().data());

    int ret = bind(udp_fd, (struct sockaddr*)&addr, sizeof(addr));
    if(this->udp_fd < 0)
    {
        qDebug() << "error bind socket";
        exit(1);
    }

    //设置该socket，可以发送广播
    int arg = 1;
#ifdef WIN32
    setsockopt(udp_fd, SOL_SOCKET, SO_BROADCAST, (char*)&arg, sizeof(arg));
#else
    setsockopt(udp_fd, SOL_SOCKET, SO_BROADCAST, &arg, sizeof(arg));
#endif

}

//发送消息
void Chat::send(const QJsonObject &obj, QString ip)
{
    QByteArray buf = QJsonDocument(obj).toJson();   //通过QJson对象得到Json数据

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = inet_addr(ip.toUtf8().data());   //0xffffffff
    sendto(udp_fd, buf.data(), buf.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
}

//回应发送消息
void Chat::sendMsg(QString content, QString ip, bool boardcast)
{
#if 0
    {
        cmd : "chat"
        broadcast: true;
        content: "msg-body"
    }
#endif

    QJsonObject obj;
    obj.insert(CMD, CHAT);
    //obj.insert(BROADCAST1, boardcast);
    obj.insert(BROADCAST1, ip.indexOf("255")!=-1);
    obj.insert(CONTENT, content);
    obj.insert(NAME, account);

    send(obj, ip);
}

//发送上线广播消息
void Chat::sendOnline()
{
    foreach (QString ip, others.keys())
    {
        delete others[ip];
    }
    others.clear();

    QJsonObject obj;
    obj.insert(CMD, QString(ONLINE));
    obj.insert(NAME, account);

    send(obj, broadcast_ip);
}
