#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QSettings>
#include <QDebug>
#include <QVariant>
#include <string>
#include <QDateTime>
#include <QMessageBox>
#include <QPainter>
#include "tcpclientsocket.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpServer(NULL)
{
    ui->setupUi(this);
   // this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//窗口初始化
void MainWindow::init()
{
    //获取配置文件信息
    getServerStatusini();
    //开启服务器
    if(!connectSocket())
    {
        return;
    }

}

//获取配置文件信息
bool MainWindow::getServerStatusini()
{
    QSettings *configIniRead = NULL;
    QString path = QCoreApplication::applicationDirPath();
    path.append("/TcpServer.ini");
    configIniRead = new QSettings(path, QSettings::IniFormat);
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    if(!configIniRead)
    {
        qDebug() <<"read ServerStatus.ini false";
        return false;
    }
    port = configIniRead->value("/SETTING/Port").toInt();
    //打印得到的结果
    qDebug() << "端口号 ="<<port;

    //读入入完成后删除指针
    delete configIniRead;
    return true;
}

bool MainWindow::connectSocket()
{
    if(port < 1024)
    {
        QMessageBox::about(NULL, QStringLiteral("提示"), QStringLiteral("本机端口号未正确配置，请进行配置"));
        return false;
    }
    if(tcpServer)
    {
        tcpServer->close();
        delete tcpServer;
    }
    tcpServer = new TcpServerSocket(this); //服务器监听类
    if(!tcpServer->init(port))//初始化是否成功
    {
        return false;
    }
    return true;
}

//键盘响应时间
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F1)
    {
    }
    if(event->key() == Qt::Key_Escape)
    {
        QMessageBox dialog(QMessageBox::Warning,
                           QStringLiteral("提示"),QStringLiteral("是否退出应用？"),
                           QMessageBox::No |
                           QMessageBox::Yes,
                           this);
        dialog.setButtonText (QMessageBox::Yes,QString(QStringLiteral("是")));
        dialog.setButtonText (QMessageBox::No,QString(QStringLiteral("否")));
        int r = dialog.exec();
        if (r == QMessageBox::Yes)
        {
            this->close();

        }
    }
    QWidget::keyPressEvent(event);
}


