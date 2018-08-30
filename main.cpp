#include "mainwindow.h"
#include <QApplication>


void myMessageOutput(QtMsgType type, const QMessageLogContext &/*context*/, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();
    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        strMsg = QString("Info:").append(msg);
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:").append(msg);
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical:").append(msg);
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal:").append(msg);
        break;
    }

    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString strMessage = QString("DateTime:%1 Message:%2").arg(strDateTime)
            .arg(strMsg);

    // 输出信息至文件中（读写、追加形式）
    QString path = QCoreApplication::applicationDirPath();
    path.append("/log.txt");
    QFile file(path);
    QFileInfo info(path);

    //如果文件大于10M则清空日志文件
    if(info.size() > 1048576)
    {
        file.remove();
    }
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //先注册自己的MessageHandler用于日志的输出
    qInstallMessageHandler(myMessageOutput);
    MainWindow w;
    w.show();

    return a.exec();
}
