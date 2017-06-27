#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QMap>
#include <QDebug>
#ifdef WIN32
#include <winsock2.h>
#define socklen_t int
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <pthread.h>
#include <error.h>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


#include "chatdef.h"

//用户信息数据结构
typedef struct
{
    QString name;
    QString ip;
}User;

/*
    负责聊天业务逻辑和数据结构
    1.name: 本机机器名， 可修改
    2.ips： 本机器ip地址列表，用于预防自己给自己发消息
    3.others: 在线的其他人
    4.udp_fd: 用于通信得的socket，使用的是UDP（广播，局域网丢包率小）
    5.tid是线程id
    6.recv_thread和run；是线程执行函数，线程负责接收报文，解析报文，处理报文，界面更新通过信号通知界面模块
*/


class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = 0);

    QString account;
    //QString name;   //机器名
    QStringList ips;    //ip列表
    //ip is key
    QMap<QString, User*> others; //线上其它用户列表

    int udp_fd;
    pthread_t tid;

    static void* recv_thread(void *ptr);    //接收线程执行函数；定义静态函数是因为this指针
    void run();
    void handleMsg(const QJsonObject &obj, QString ip);
    void addUser(QString ip, QString name);

    //初始化
    void init();
    QString getSysName();
    QStringList getSysIps();

    //发送数据
    //send函数，主线程和子线都会调用
    void create_socket(QString ip);
    QString broadcast_ip;
    void send(const QJsonObject &obj, QString ip);
    void sendMsg(QString content, QString ip, bool boardcast);
    void sendOnline();  //上线广播消息

signals:
    //信号是用于通知界面模块
    void sigNewUser(QString name, QString ip);
    void sigNewContent(QString name, QString content, bool broadcast);
public slots:
};

#endif // CHAT_H
