#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent)
    : QTcpServer{parent}
{

}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;      //单例模式，声明静态对象，无论调用多少次，有且只有一个对象
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    //qDebug()<<"服务器接受了来自客户端连接";
    MyTcpSocket *pTcpSocket=new MyTcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);//接收来自客户端的socket
    m_tcpSocketList.append(pTcpSocket);

    connect(pTcpSocket,SIGNAL(offLine(MyTcpSocket*)),this,SLOT(deleteSocket()));
}

void MyTcpServer::resend(const char *hisname, PDU *pdu)
{
    if(NULL==hisname ||pdu==NULL){
        return ;
    }   //判断形式参数有效性
    QString strName=hisname;
    for(int i=0;i<m_tcpSocketList.size();i++){
        if(strName==m_tcpSocketList.at(i)->getName()){      //寻找目标用户的socket
            m_tcpSocketList.at(i)->write((char *)pdu,pdu->uiPDUlen);  //目标用户发送给当前客户端登录用户的socket,并向其转发当前用户的请求添加好友的pdu
            break;
        }
    }
}



void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    QList<MyTcpSocket *>::Iterator iter=m_tcpSocketList.begin();
    for(;iter!=m_tcpSocketList.end();iter++){
        if(mysocket==*iter){
            delete *iter;
            *iter=NULL;
            m_tcpSocketList.erase(iter);   //从QLIST里面移除这个项
            break;
        }
    }
    for(int i=0;i<m_tcpSocketList.size();i++){
        //qDebug()<<m_tcpSocketList.at(i)->getName();//打印移除之后的项
    }
}
