﻿#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT //添加这个宏是为了实现信号和槽的通信
public:
    TcpClientSocket(QObject *parent = 0);
    QString getUser();
protected slots:
    void receivedata();//处理readyRead信号读取数据
    void slotclientdisconnected();//客户端断开连接触发disconnected信号，这个槽函数用来处理这个信号

signals:
    void chat(QString msg,QString user);//聊天
    void drawPaint();//绘制
    void clientdisconnected(int); //告诉server有客户端断开连接
private:
    QString user;

};

#endif // TCPCLIENTSOCKET_H
