#include "tcpserversocket.h"
#include <QHostAddress>

TcpServerSocket::TcpServerSocket(QObject *parent):QTcpServer(parent)
{

}

bool TcpServerSocket::init(int port)
{
    if(!listen(QHostAddress::Any,port)) //服务器监听本机所有IP端口
    {
        qDebug() << errorString(); //Debug出错误信息
        close(); //关闭监听
        return false;
    }
    return true;
}

void TcpServerSocket::incomingConnection(int socketDescriptor)
{
    TcpClientSocket *tcpclientsocket = new TcpClientSocket(this);//只要有新的连接就生成一个新的通信套接字
    //将新创建的通信套接字描述符指定为参数socketdescriptor
    tcpclientsocket->setSocketDescriptor(socketDescriptor);

    //将这个套接字加入客户端套接字列表中
    tcpclientsocketlist.append(tcpclientsocket);

    //接收到tcpclientsocket发送过来的更新界面的信号
    connect(tcpclientsocket, &TcpClientSocket::clientdisconnected,
            this, &TcpServerSocket::slotclientdisconnect);

    connect(tcpclientsocket, &TcpClientSocket::chat,
            this, &TcpServerSocket::sliotupdateserver);

}

void TcpServerSocket::sliotupdateserver(QString msg,QString user)
{
    //将收到的信息发送给每个客户端,从套接字列表中找到需要接收的套接字
    for(int i = 0; i < tcpclientsocketlist.count(); i++)
    {
        TcpClientSocket *item = tcpclientsocketlist.at(i);
        if(item->getUser() != user)
        {

            item->write(msg.toUtf8().data());
        }
    }

}

void TcpServerSocket::slotclientdisconnect(int descriptor)
{
    for(int i = 0; i < tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            tcpclientsocketlist.removeAt(i);//如果有客户端断开连接， 就将列表中的套接字删除
            break;
        }
    }
}
