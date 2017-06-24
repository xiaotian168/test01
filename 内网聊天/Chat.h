#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QList>
#ifdef WIN32
#include <winsock2.h>
#define socklen_t int
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <pthread.h>
#include <errno.h>

#include <QDebug>

#include "chatdef.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QMap>

typedef struct
{
    QString name;
    QString ip;
} User;

/*
    负责聊天业务逻辑和数据结构
    1. name：本机机器名，可修改
    2. ips：本机器ip地址列表，用于预防自己给自己发消息
    3. others：在线的其他人
    4. udp_fd：用于通信的socket，使用的是UDP（广播，局域网丢包可能性小）
    5. tid是线程id
    6. recv_thread和run：是线程执行函数，线程负责接收报文，解析报文，处理报文，界面更新通过信号通知界面模块
*/

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = nullptr);

    QString name;
    QStringList ips;
    // ip is key
    QMap<QString, User*> others;

    int udp_fd;
    pthread_t tid;

    static void* recv_thread(void* ptr);
    void run();
    void handleMsg(const QJsonObject& obj, QString ip);
    void addUser(QString ip, QString name);

    // 初始化
    void init();
    QString getSysName();
    QStringList getSysIps();

    // 发送数据
    // send函数，两个线程都会调用
    void send(const QJsonObject& obj, QString ip);
    void sendMsg(QString content, QString ip, bool boardcast);

signals:
    // 信号是用于通知界面模块
    void sigNewUser(QString name, QString ip);
    void sigNewContent(QString name, QString content, bool boardcast);

public slots:
};

#endif // CHAT_H
