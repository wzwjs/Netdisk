#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QWidget>
#include <QFile>
#include <QTcpServer>
#include <QHostAddress>
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include "mytcpserver.h"
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class NetworkServer;
}
QT_END_NAMESPACE

class NetworkServer : public QWidget
{
    Q_OBJECT

public:
    NetworkServer(QWidget *parent = nullptr);
    ~NetworkServer();
    void loadConfig();
public slots:
    //void do_newConnect();
    //void readClientData();


private:
    Ui::NetworkServer *ui;
    //配置文件读取
    QString m_strIP;      //保存配置文件的IP
    quint16 m_usPort;       //保存配置文件的端口
    QTcpServer *M_SERVER;
    //QTcpSocket *s_socket;
};
#endif // NETWORKSERVER_H
