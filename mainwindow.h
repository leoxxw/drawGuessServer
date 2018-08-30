#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <Qtsql/QSqlDatabase>
#include <Qtsql/QSqlQuery>
#include <QtSql/QSql>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>
#include "tcpserversocket.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void init();

    bool getServerStatusini();

    bool connectSocket();

private:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    TcpServerSocket *tcpServer;
    int     port;
};

#endif // MAINWINDOW_H
