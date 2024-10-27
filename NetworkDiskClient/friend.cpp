#include "friend.h"
#include "protocol.h"
#include "networkdisk.h"
#include <QInputDialog>
#include <QMessageBox>
#include "privatechat.h"
Friend &Friend::getinstance()
{
    static Friend instance;
    return instance;
}

Friend::Friend(QWidget *parent)
    : QWidget{parent}
{
    //resize(657,496);
    setFixedSize(657,496);
    m_pShowMsgTE =new QTextEdit;  //显示信息的
    m_pFriendListWidget=new QListWidget;//显示好友列表
    m_pInputMsgLE=new QLineEdit;        //显示信息输入框

    m_pDelFriendDB=new QPushButton("删除好友");
    m_FlushFriendpB=new QPushButton("刷新好友");
    m_pShowOnlineUsrPB=new QPushButton("显示在线用户");  //
    m_pSearchUsrPB=new QPushButton("查找用户");

    m_pMsgSendDB=new QPushButton("信息发送");       //发信息 就是群聊
    m_poPrivateChatPB=new QPushButton("私聊");//私聊
    QString login_name=Networkdisk::getInstance().loginName();
    m_current_name=new QLabel(login_name);
    //左边的按钮搞一个垂直布局
    QVBoxLayout *pRightPBVBL=new QVBoxLayout;
    pRightPBVBL->addWidget(m_current_name);
    pRightPBVBL->addWidget(m_pDelFriendDB);
    pRightPBVBL->addWidget(m_FlushFriendpB);
    pRightPBVBL->addWidget(m_pShowOnlineUsrPB);
    pRightPBVBL->addWidget(m_pSearchUsrPB);
    pRightPBVBL->addWidget(m_poPrivateChatPB);


    //右边的显示跟左边的搞一个水平布局
    QHBoxLayout *pTopHBL=new QHBoxLayout;
    pTopHBL->addWidget(m_pShowMsgTE);
    pTopHBL->addWidget(m_pFriendListWidget);
    pTopHBL->addLayout(pRightPBVBL);

    //信息输入框的搞一个水平布局
    QHBoxLayout *pMsgHBL=new QHBoxLayout;
    pMsgHBL->addWidget(m_pInputMsgLE);
    pMsgHBL->addWidget(m_pMsgSendDB);

    //online
    m_pOnline=new Online;

    //垂直布局
    QVBoxLayout *pMain=new QVBoxLayout;
    pMain->addLayout(pTopHBL);
    pMain->addLayout(pMsgHBL);
    pMain->addLayout(pMsgHBL);

    pMain->addWidget(m_pOnline,100);
    m_pOnline->hide();//隐藏online界面

    setLayout(pMain);


    //显示在线用户的信号槽的关联
    connect(m_pShowOnlineUsrPB,SIGNAL(clicked(bool)),this,SLOT(showOnline()));
    //查找用户的信号槽关联
    connect(m_pSearchUsrPB,SIGNAL(clicked(bool)),this,SLOT(searchUsr()));
    //添加好友的信号槽关联

    //刷新好友列表的信号槽关联
    connect(m_FlushFriendpB,SIGNAL(clicked(bool)),this,SLOT(flushFriend()));

    //删除好友的信号槽关联
    connect(m_pDelFriendDB,SIGNAL(clicked(bool)),this,SLOT(delFriend()));
    //私聊好友
    connect(m_poPrivateChatPB,SIGNAL(clicked(bool)),this,SLOT(privateChat()));
    connect(m_pMsgSendDB,SIGNAL(clicked(bool)),this,SLOT(groupChat()));
}

void Friend::showAllOnLineUsr(PDU *pdu)
{
    if(NULL==pdu){
        return ;
    }
    //将这些名字显示到On
    m_pOnline->showUsr(pdu);
}

void Friend::updataFriendList(PDU *pdu)
{
    qDebug()<<"run this ok";
    if(NULL==pdu){
        return ;
    }
    //判断清理消息的无效性
    //将对应的控件刷新到界面上去。
    char friend_name[32]={'\0'};
    uint msglen=pdu->uiMsgLen/32;   //总共的好友人数
    qDebug()<<msglen;
    //解压pdu
    //循环的将名字拷贝到该pdu
    //循环的将好友消息刷新到界面上去
    for(uint i=0;i<msglen;i++){
        memcpy(friend_name,(char *)(pdu->caMsg)+i*32,32);
        m_pFriendListWidget->addItem(friend_name);
        //qDebug()<<"friend name is "<<friend_name;
    }




}

void Friend::showOnline()
{
    if(m_pOnline->isHidden()){
        m_pOnline->show();
        //产生pdu数据单元,和其数据类型
        PDU *pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);  //发送请求数据
        free(pdu);
        pdu=NULL;

    }
    else {
        m_pOnline->hide();
    }
}

//查找用户
void Friend::searchUsr()
{
    //弹出一个输入框去搜索用户
    QString m_strSearchName= QInputDialog::getText(this,"搜索","用户名");
    //qDebug()<<m_strSearchName;
    if(!m_strSearchName.isEmpty()){
        //uint size=name.size();
        PDU *pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_SEARCH_USR_REQUEST;
        strncpy(pdu->caData,m_strSearchName.toStdString().c_str(),32);
        Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);  //发送请求数据
        free(pdu);
        pdu=NULL;
    }
    else{
        QMessageBox::warning(this,"警告","","当前输入有误");
    }

}

//刷新好友列表
void Friend::flushFriend()
{   //qDebug()<<"run this";
    //第一，将自己的用户名发送给服务器的数据库查询，自己的好友列表有哪一些
    QString login_name=Networkdisk::getInstance().loginName();
    qDebug()<<login_name;
    PDU *pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    memcpy(pdu->caData,login_name.toStdString().c_str(),login_name.size());
    Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);  //发送请求数据
    free(pdu);
    pdu=NULL;

    //在缓冲区收到数据的时候，将对应的消息显示起来，即可
}
//删除用户
void Friend::delFriend()
{
    if(m_pFriendListWidget->currentItem()->text()!=NULL){
        QString name=m_pFriendListWidget->currentItem()->text();
        //获取登录用户名，要删除好友的用户名
        QString login_name=Networkdisk::getInstance().loginName();  //当前登录用户的用户名

        //向服务器发送一个关于删除好友的消息
        PDU *pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_DELETE_REQUEST;
        memcpy(pdu->caData,login_name.toStdString().c_str(),32);
        memcpy(pdu->caData+32,name.toStdString().c_str(),32);
        Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);  //发送请求数据
        free(pdu);
        pdu=NULL;
    }

    //QString name=m_pFriendListWidget->currentItem()->text();    //获取当前选中要删除用户的用户名

    //服务器接收，并在数据库中删除好友，并将新的好友列表返回给客户端

    //客户端接收到新的好友列表，并刷新到界面上

}

void Friend::privateChat()
{
    if(NULL!=m_pFriendListWidget->currentItem()){
        //获得私聊对象的名字
        QString str_chatname=m_pFriendListWidget->currentItem()->text();
        PrivateChat::getinstance().setChatName(str_chatname);
        if(PrivateChat::getinstance().isHidden()){
            PrivateChat::getinstance().show();
        }
    }else
    {
        QMessageBox::warning(NULL,"私聊","请选择私聊的对象");
    }
}

void Friend::groupChat()
{
    QString msg=m_pInputMsgLE->text();//获取当前输入的信息发送给所有的好友
    if(!msg.isEmpty()){
        PDU *pdu=mkPDU(msg.size()+1);
        pdu->uiMsgType=ENUM_MSG_TYPE_GROUP_CHAT_REQUEST;
        QString strName=Networkdisk::getInstance().loginName();
        memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
        memcpy((char *)pdu->caMsg,msg.toStdString().c_str(),msg.size());
        Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);
        free(pdu);
        pdu=NULL;
    }
    else{
        QMessageBox::warning(NULL,"群聊","输入的信息不能为空");
    }
}

void Friend::show_group_msg(PDU *pdu)
{
    //qDebug()<<"run this ok";
    // if(NULL==pdu){
    //     return ;
    // }
    //判断清理消息的无效性
    //将对应的控件刷新到界面上去。
    QString strMsg=QString("%1 say:%2").arg(pdu->caData).arg((char *)pdu->caMsg);
    m_pShowMsgTE->append(strMsg);

    // qDebug()<<" 3 do you run this ?";
    // strncpy((char *)msg)
    // memcpy(msg,(pdu->caMsg),pdu->uiMsgLen);
    // qDebug()<<msg;
    // m_pShowMsgTE->append((char*)pdu->caMsg);
    // qDebug()<<(char*)pdu->caMsg;
    // qDebug()<<" 4 do you run this ?";
    // // //解压pdu
    // //循环的将名字拷贝到该pdu
    // //循环的将好友消息刷新到界面上去
    // for(uint i=0;i<msglen;i++){
    //     memcpy(friend_name,(char *)(pdu->caMsg)+i*32,32);
    //     m_pFriendListWidget->addItem(friend_name);
    //     //qDebug()<<"friend name is "<<friend_name;
    // }


}
