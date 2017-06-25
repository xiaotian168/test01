#include "mainwindow.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>

/*
    主窗口构造函数
    1.连接对象成员Chat（用户聊天业务的类）的信号和自己的槽函数
    2.初始化对象成员Chat
    3.绘制主窗口界面信息
    4.连接消息发送按钮的点击和槽函数
*/
MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    //连接chat对象的信号和本地的槽函数
    connect(&chat, SIGNAL(sigNewUser(QString, QString)),
                          this, SLOT(onNewUser(QString,QString)));
    connect(&chat, SIGNAL(sigNewContent(QString, QString, bool)),
                          this, SLOT(onNewContent(QString,QString,bool)));

    chat.init();

    //主窗口界面设置
    QVBoxLayout *m = new QVBoxLayout(this); //垂直布局(将主窗口分为左右两部分) -->左边的垂直布局
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(userList = new QListWidget);
    m->addWidget(splitter);

    QWidget *container;
    splitter->addWidget(container = new QWidget);

    QVBoxLayout *vBox = new QVBoxLayout(container); //右边的垂直布局
    QSplitter *Right = new QSplitter(Qt::Vertical, container);  //设置分割窗口的上下分割属性；默认是左右
    Right->addWidget(this->msgShow = new QTextBrowser);
    Right->setStretchFactor(0,3);   //设置第一个（下标为0的）窗口占3份大小   --窗口下标从0开始
    Right->addWidget(this->msgInput = new QTextEdit);
    Right->setStretchFactor(1,1);   //设置第二个窗口占3份大小
    //vBox->addWidget(this->msgShow = new QTextBrowser, 3);  //添加信息浏览框到垂直布局
    //vBox->addWidget(this->msgInput = new QTextEdit, 1);    //添加文本编辑框到垂直布局
    vBox->addWidget(Right);

    QHBoxLayout *hBox = new QHBoxLayout;
    hBox->addStretch(1);    //加个弹簧
    hBox->addWidget(this->setup = new QPushButton("设置"));
    hBox->addWidget(this->sendMsg = new QPushButton("发送"));
    vBox->addLayout(hBox);

    vBox->setMargin(0); //布局边框距离设置为0
    hBox->setMargin(0);

    //设置连接  按钮的点击信号和槽函数
    connect(sendMsg, SIGNAL(clicked()), this, SLOT(onSend()));
}

//发送信息给指定的用户；并且更新界面
void MainWindow::onSend()
{
    //等到用户的ip地址
    QString text = userList->currentItem()->text(); //获取当前选中项目的文本信息
    qDebug() << "item text is:" << text;    //name@ip

    QStringList stringList = text.split('@');   //分割出name和ip信息，并且保存在stringList中
    if(stringList.length() != 2)    //判断用户和ip信息是否合法
    {
        qDebug() << stringList;
        return;
    }
    QString ip = stringList.at(1);  //获取ip信息

    //得到发送内容信息
    QString content = msgInput->toPlainText();
    if(content.length() == 0)   //判断输入内容为空则直接忽略
    {
        return;
    }

    chat.sendMsg(content, ip, false);   //调用聊天业务对象发送信息

    //整理更新界面
    msgInput->clear();  //消息发送完后，清空编辑输入框内容
    msgShow->append("我说: " + content);    //消息浏览记录框追加聊天信息
}

//保存新用户上线信息，广播信息并且更新界面
void MainWindow::onNewUser(QString name, QString ip)
{
    int count = this->userList->count();  //获得用户列表中用户数量
    for(int i = 0; i < count; ++i)
    {
        QListWidgetItem *item = userList->item(i);  //获取每个用户列表对应的条目信息
        if(item->text().indexOf(ip) != -1)  //判断上线用户和用户列表中用户相同
        {
            qDebug() << "remove same user";
            // userList->removeItemWidget(item);
            //  userList->update();
            delete userList->takeItem(i);
            break;
        }
    }

    this->userList->addItem(name +  "@" + ip);    //新用户加入到用户列表条目窗口中
}

//添加聊天记录；更新信息浏览窗口的界面信息
void MainWindow::onNewContent(QString name, QString content, bool boardcast)
{
    msgShow->append(name + "说: " + content);
}
