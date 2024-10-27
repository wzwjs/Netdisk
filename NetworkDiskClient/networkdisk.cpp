#include "networkdisk.h"
#include "ui_networkdisk.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QApplication>
#include "networkdisk.h"
#include "protocol.h"
#include "privatechat.h"

/*
    10月6日
    static OpeWidget &getInstance();//这个是什么东西?

    */

Networkdisk::Networkdisk(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Networkdisk)
{
    ui->setupUi(this);
    loadConfig();

    //resize(500,250);//放大界面
    //连接服务器
    //setFixedSize();//固定尺寸
    m_tcpsocket.connectToHost(QHostAddress(m_strIP),m_usPort);
    //QT4 写法
    connect(&m_tcpsocket,SIGNAL(connected()),this,SLOT(showConnect()));
    //connect(&m_tcpsocket,&QTcpSocket::connected,this,&Networkdisk::showConnect);
    //接收来自服务端的信号槽函数响应
    connect(&m_tcpsocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));

}

Networkdisk::~Networkdisk()
{
    delete ui;

}
//读取配置文件
void Networkdisk::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray data=file.readAll();
        QString strData=data.toStdString().c_str();  //不能直接转换成为qstring 需要转化成为char *类型
        //qDebug()<<strData;
        file.close();

        //拿配置文件里面的ｉｐ和端口
        strData.replace("\r\n"," ");
        //qDebug()<<strData;
        QStringList list;
        list = strData.split(" ");
        for(int i=0;i<list.size();i++){
            //qDebug()<<"--->\n"<<list[i];
            m_strIP=list[0];
            m_usPort=list[1].toUShort();
           // qDebug()<<m_strIP<<"   "<<m_usPort;
        }
    }
    else{
        QMessageBox::critical(this,"读取配置文件","读取配置文件失败");
    }
}

Networkdisk &Networkdisk::getInstance()
{
    static Networkdisk instance;    //产生静态的对象
    return instance;
}

QTcpSocket &Networkdisk::getTcpSocket()
{
    return m_tcpsocket;     //做一个公开的接口函数专门获得类里面的私有函数，将私有函数放开
}

QString Networkdisk::loginName()
{
    return m_StrLogname;//将登录的用户名返回出去
}

QString Networkdisk::CurPath()
{
    return m_strCurPath;
}

//signal handle function
void Networkdisk::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}

void Networkdisk::recvMsg()
{

    //qDebug()<<m_tcpsocket.bytesAvailable();
    //收数据
    uint uiPDUlen=0;                                         //这是在函数内部声明这个协议的所有数据大小
    m_tcpsocket.read((char *)&uiPDUlen,sizeof(uint));             //如何将这个协议里面内部的数据大小保存到这个uiPDUlen内
    uint uiMsgLen=uiPDUlen-sizeof(PDU);//实际消息的长度
    PDU *pdu=mkPDU(uiMsgLen);  //利用这个PDU去接收剩余的数据
    m_tcpsocket.read((char *)pdu+sizeof(uint),uiPDUlen-sizeof(uint));
    //qDebug()<<pdu->uiMsgType<<(char *)(pdu->caMsg);           //打印数据类型


    //收到消息之后，判断其的消息类型
    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
    {//如果是响应请求，就将消息搞出来

        if(0==strcmp(pdu->caData,REGIST_OK)){
            QMessageBox::information(this,"注册",REGIST_OK);
        }
        else if(0==strcmp(pdu->caData,REGIST_FAILED)){
            QMessageBox::warning(this,"注册",REGIST_FAILED);
        }

        break;
    }

    case ENUM_MSG_TYPE_LOGIN_RESPOND:{
        if(0==strcmp(pdu->caData,LOING_OK)){
            QMessageBox::information(this,"登录",LOING_OK);
            //如果登录成功，就跳转显示界面
            OpeWidget::getInstance().show();//通过封装操作窗口的实例为引用的单例模式去显示好友界面，再去隐藏调原来的那个登录界面
            this->hide();//this是一个隐藏的形式参数，可写，可不写去隐藏那个原来的对象
            //进入以你名字命名的文件夹
            m_strCurPath=QString("./%1").arg(m_StrLogname);//通过登录的名字，在登录的时候，就默认生成应该以当前名字命名的文件夹

        }
        else if(0==strcmp(pdu->caData,LOING_FAILED)){
            QMessageBox::warning(this,"登录",LOING_FAILED);
        }
    }
    //收到显示在线用户回复的消息
    case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:{
        //接收数据
        //循环提取QStringList的数据
        OpeWidget::getInstance().getfriend()->showAllOnLineUsr(pdu);
    }

    //返回服务端关于查询查找用户的请求响应
    case ENUM_MSG_TYPE_SEARCH_USR_RESPOND:
    {
        //QString tmp=pdu->caData;
        if(0==strcmp(SEARCH_USR_NO,pdu->caData)){
            QMessageBox::information(this,"查找用户",QString("%1: 不存在").arg(OpeWidget::getInstance().getfriend()->m_strSearchName));
        }
        else if(0==strcmp(SEARCH_USR_OFFLINE,pdu->caData)){
            QMessageBox::information(this,"查找用户","该用户不在线");
        }
        else if(0==strcmp(SEARCH_USR_ONLINE,pdu->caData)){
           QMessageBox::information(this,"查找用户","该用户在线");
        }
    }
    //针对于其他客户端想加当前客户端的好友请求
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
        //服务器原样转发过来的
        // char his_Name[32]={'\0'};
        // char my_Name[32]={'\0'};
        char his_Name[32]={'\0'};        //添加者客户端的名字
        char my_Name[32]={'\0'};         //被添加者客户端的名字
        strncpy(my_Name,pdu->caData,32);    //被添加者客户端的名字

        strncpy(his_Name,pdu->caData+32,32);//接收客户端的数据，接收客户端的请求的添加好友的姓名和登录者的姓名
        int ret=QMessageBox::information(this,"添加好友",QString("%1 want add %2  as friend").arg(his_Name).arg(my_Name),QMessageBox::Yes,QMessageBox::No);
        PDU *respdu=mkPDU(0);
        memcpy(respdu->caData,pdu->caData,32);

        if(ret==QMessageBox::Yes){
            //客户端产生回复同意其他客户端的加好友的请求
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_AGREE;

        }
        else {
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;
        }
        m_tcpsocket.write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }
    //针对于添加好友的非1回复操作
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:{
        QMessageBox::information(NULL,"添加好友",pdu->caData);
        break;
    }
    //针对于被添加用户不同意添加用户的好友申请
    case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:{
        char his_Name[32]={'\0'};        //被添加者客户端的名字

        strncpy(his_Name,pdu->caData,32);    //被添加者客户端的名字


        QMessageBox::information(this,"添加好友",QString("%1  add failed as friend").arg(his_Name));
    }
    //针对于被添加用户同意添加好友的好友申请
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:{

    }

    //针对于刷新用户的回复的处理
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:{
        qDebug()<<"run this handle respond";
        OpeWidget::getInstance().getfriend()->updataFriendList(pdu);
        break;
    }
    //删除好友后，服务器刷新的好友列表到界面上去
    case ENUM_MSG_TYPE_DELETE_RESPOND:{
        QMessageBox::information(NULL,"删除好友","成功删除好友");
        break;
    }
    //删除好友，对面好友的回复
    case ENUM_MSG_TYPE_DELETE_REQUEST:{
        char caName[32]={'\0'};
        memcpy(caName,pdu->caData,32);
        QMessageBox::information(NULL,"删除好友",QString("%1删除你作为他的好友").arg(caName));

    }
        //私聊好友的请求
    case ENUM_MSG_TYPE_PRIVATE_REQUEST:{

        if(PrivateChat::getinstance().isHidden()){
            PrivateChat::getinstance().show();
        }
        char caSendName[32]={'\0'};
        memcpy(caSendName,pdu->caData,32);
        QString strSendName=caSendName;
        PrivateChat::getinstance().setChatName(strSendName);
        PrivateChat::getinstance().updateMsg(pdu);
        break;
    }
        //群聊消息处理
    case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:{
        //将消息刷到消息框
        OpeWidget::getInstance().getfriend()->show_group_msg(pdu);
        //Friend::getinstance().show_group_msg(pdu);
        break;
    }
    case ENUM_MSG_TYPE_CREATE_PATH_RESPOND:{
        QMessageBox::information(this,"创建文件",pdu->caData);
        break;
    }
      //处理刷新文件的响应
    case ENUM_MSG_TYPE_FLUSH_FILES_RESPOND:{
        //就是将这些文件循环的展示的界面上
        OpeWidget::getInstance().getBook()->updateFileList(pdu);
        //如果没有失败的话
        QString strEnterDir = OpeWidget::getInstance().getBook()->enterDir();

        if(NULL != strEnterDir)
        {

            m_strCurPath = m_strCurPath + "/" + strEnterDir;
            qDebug() << "进入的文件夹路径:" << m_strCurPath << "文件夹名字：" << strEnterDir;

        }
        break;
    }
    //针对于删除文件的响应
    case ENUM_MSG_TYPE_DEL_DIR_RESPOND:{
        QMessageBox::information(this,"删除文件夹",pdu->caData);
        break;
    }
    //针对于重命名文件的响应
    case ENUM_MSG_TYPE_RENAME_FILE_RESPOND:{
        QMessageBox::information(this,"删除文件夹",pdu->caData);
    }
        //进入文件夹的失败的响应
    case ENUM_MSG_TYPE_ENTER_DIR_RESPOND:{
        OpeWidget::getInstance().getBook()->clearEnterDir();
        QMessageBox::information(this,"进入文件夹",pdu->caData);

    }
    default:
        break;
    }

    free(pdu);
    pdu=NULL;

}



// void Networkdisk::on_send_pb_clicked()
// {
//     QString strmsg=ui->lineEdit->text();
//     //qDebug()<<strmsg;
//     if(strmsg.isEmpty()){
//          QMessageBox::warning(this,"发送数据","发送数据失败");
//     }
//     else{
//         PDU *pdu=mkPDU(strmsg.size());//申请协议数据空间
//         pdu->uiMsgType=8888;            //定义消息类型
//         memcpy(pdu->caMsg,strmsg.toStdString().c_str(),strmsg.size());  //将要发送的数据拷贝到对应的内存空间
//         //QByteArray byteArray = strmsg.toUtf8(); // 转换为 UTF-8 编码的字节数组
//         m_tcpsocket.write((char *)pdu,pdu->uiPDUlen);
//         free(pdu);
//         pdu=NULL;
//     }

// }


void Networkdisk::on_login_pb_clicked()
{
    //登录
    //获得输入的用户名和密码
    QString strName=ui->name_le->text();

    QString strPwd=ui->pwd_le->text();
    if(strName.isEmpty()){
        QMessageBox::warning(this,"用户名","用户名为空");
    }
    else if(strPwd.isEmpty()){
        QMessageBox::warning(this,"密码","密码为空");

    }
    else{
        //登录的请求
        m_StrLogname=strName;//将登录的用户名保存
        PDU *pdu=mkPDU(0);//实际消息为0
        pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_REQUEST; //登录请求
        strncpy(pdu->caData,strName.toStdString().c_str(),32);     //前32位放姓名
        strncpy(pdu->caData+32,strName.toStdString().c_str(),32);   //后32位放密码
        m_tcpsocket.write((char *)pdu,pdu->uiPDUlen);
        free(pdu);
        pdu=NULL;
        // strName=(char *)strName;
        // //向服务器发送用户名和密码
        // //向数据库里面的用户名和密码比对
        // m_tcpsocket.write(strName.toStdString());
    }
}


void Networkdisk::on_regist_pb_clicked()
{
    //获得输入的用户名和密码
    QString strName=ui->name_le->text();
    QString strPwd=ui->pwd_le->text();
    if(strName.isEmpty()){
        QMessageBox::warning(this,"用户名","用户名为空");
    }
    else if(strPwd.isEmpty()){
        QMessageBox::warning(this,"密码","密码为空");

    }
    else{
        //注册的请求

        PDU *pdu=mkPDU(0);//实际消息为0
        pdu->uiMsgType=ENUM_MSG_TYPE_REGIST_REQUEST; //注册请求
        strncpy(pdu->caData,strName.toStdString().c_str(),32);     //前32位放姓名
        strncpy(pdu->caData+32,strName.toStdString().c_str(),32);   //后32位放密码
        m_tcpsocket.write((char *)pdu,pdu->uiPDUlen);
        free(pdu);
        pdu=NULL;
        // strName=(char *)strName;
        // //向服务器发送用户名和密码
        // //向数据库里面的用户名和密码比对
        // m_tcpsocket.write(strName.toStdString());
    }


}


void Networkdisk::on_cancel_pb_clicked()
{

}

