#ifndef NETWORKDISK_H
#define NETWORKDISK_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpSocket>
#include <QByteArray>
#include "protocol.h"
#include "opewidget.h"  //操作窗口的类


QT_BEGIN_NAMESPACE
namespace Ui {
class Networkdisk;
}
QT_END_NAMESPACE

class Networkdisk : public QWidget
{
    Q_OBJECT

public:
    Networkdisk(QWidget *parent = nullptr);
    ~Networkdisk();
    void loadConfig();

    static Networkdisk &getInstance();//产生单例对象
    QTcpSocket &getTcpSocket();
    QString loginName();
    QString CurPath();
public slots:
    void showConnect();//显示连接状态
    void recvMsg();     //接收来自服务端的TCPSOCKET的数据
private slots:
    //void on_send_pb_clicked();

    void on_login_pb_clicked();

    void on_regist_pb_clicked();

    void on_cancel_pb_clicked();    //注销按钮的响应函数

private:
    Ui::Networkdisk *ui;
    //配置文件读取
    QString m_strIP;      //保存配置文件的IP
    quint16 m_usPort;       //保存配置文件的端口

    //连接服务器
    QTcpSocket m_tcpsocket;
    //保存用户名，添加好友请求的
    QString m_StrLogname;
    QString m_strCurPath;
};
#endif // NETWORKDISK_H
