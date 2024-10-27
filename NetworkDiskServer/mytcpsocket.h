#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include <QMessageBox>
#include <QDir>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);
    QString getName();
signals:
    void offLine(MyTcpSocket *mysocket);     //下线的信号，用与删除QTcpServer类中的一个QTcpSocket
public slots:
    void recvMsg();    //接收数据
    void clientOffLine();   //处理客户端下线的槽函数
private:
    QString m_strName;
};

#endif // MYTCPSOCKET_H
