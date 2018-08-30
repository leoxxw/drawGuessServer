#include "tcpclientsocket.h"
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonParseError>

TcpClientSocket::TcpClientSocket(QObject *parent):
    QTcpSocket(parent)
{
    //客户端发送数据过来就会触发readyRead信号
    connect(this, &TcpClientSocket::readyRead, this, &TcpClientSocket::receivedata);
    connect(this, &TcpClientSocket::disconnected, this, &TcpClientSocket::slotclientdisconnected);
}

QString TcpClientSocket::getUser()
{
    return user;
}

void TcpClientSocket::receivedata()
{
    //读取全部数据； 返回类型为QByteArray
    QByteArray bytes = readAll();//获取接受的信息
    QString ip = this->peerAddress().toString();//获取IP
  //  QString srvName = peerName();//获取服务器名字

    //正则匹配IP（ip的获取不是标准格式）
    QRegExp rx2("((?:(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d)\\.){3}(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d))");
    rx2.indexIn(ip);
    ip = rx2.cap(0);

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(bytes, &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << QStringLiteral("解析Json失败");
        return;
    }

    // 解析Json
    QString type;
    if (doucment.isObject())
    {
        QJsonObject obj = doucment.object();
        QJsonValue value;
        if (obj.contains("Name"))
        {
            value = obj.take("Name");
            if (value.isString())
            {
                user = value.toString();
            }
        }
        if (obj.contains("Type"))
        {
            value = obj.take("Type");
            if (value.isString())
            {
                type = value.toString();
            }
        }
    }
    if(type != "login")
    {
        QString msg = bytes;
        emit chat(msg,user);
    }
}

void TcpClientSocket::slotclientdisconnected()
{
    emit clientdisconnected(this->socketDescriptor());
}
