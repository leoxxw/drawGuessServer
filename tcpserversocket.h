#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H
#include <QObject>
#include <QList>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include "tcpclientsocket.h"

class TcpServerSocket : public QTcpServer
{
    Q_OBJECT //为了实现信号和槽的通信
public:
    TcpServerSocket(QObject *parent = 0);
    bool init(int port = 0);
protected:
    void incomingConnection(int socketDescriptor);//只要出现一个新的连接，就会自动调用这个函数
protected slots:
    void sliotupdateserver(QString,QString user);//用来处理tcpclient发过来的信号
    void slotclientdisconnect(int);

signals:
    void updateserver(QString);//发送信号给界面，让界面更新信息

private:
     QList<TcpClientSocket*> tcpclientsocketlist;

};


#endif // TCPSERVERSOCKET_H
