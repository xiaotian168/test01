#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>
#include "chat.h"

/*
    负责界面更新，界面：
    1.用户列表 userList
      显示所有上线用户名和ip地址信息
    2.消息输出 msgShow
      显示消息
    3.消息输入 msgInput
    4.发送按钮 sendMsg
    5.设置按钮 setup

    外部接口：
    void onNewUser(QString name, QString ip);
    当有新用户加入聊天，该函数用于更新界面

    void onNewContent(QString name, QString content, bool boardcast);
    当接收到新消息时，负责更新界面

    功能：
    用户输入消息， 发送消息
*/

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    Chat chat;

    //left
    QListWidget *userList;  //用户列表窗口，里面可以添加用户条目
    //right
    QTextBrowser *msgShow;  //消息浏览器
    QTextEdit   *msgInput;  //消息输入编辑框
    QPushButton *sendMsg;   //消息发送按钮
    QPushButton *setup; //设置按钮

signals:

//外部调用的槽函数
public slots:
    void onSend();  //（主动发送信息时）更新消息浏览器的界面信息

    void onNewUser(QString name, QString ip);   //新用户上线,更新界面显示用户名和ip信息
    void onNewContent(QString name, QString content, bool boardcast);   //（被动接收信息时）更新消息浏览器的界面信息
};

#endif // MAINWINDOW_H
