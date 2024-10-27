#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <mytcpsocket.h>
#include "protocol.h"
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);

    //单例模式
    static MyTcpServer &getInstance();
    virtual void incomingConnection(qintptr socketDescriptor);

    void resend(const char * hisname,PDU *pdu);//根据名字查找socket,查找名字成功，进行转发
public slots:
    void deleteSocket(MyTcpSocket *mysocket);  //从QList<MyTcpSocket *>里面删除当前下线的客户端的socket
private:
    //所有的socket
    QList<MyTcpSocket *> m_tcpSocketList;
};

#endif // MYTCPSERVER_H
