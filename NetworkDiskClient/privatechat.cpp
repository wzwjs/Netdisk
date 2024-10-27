#include "privatechat.h"
#include "ui_privatechat.h"
#include <protocol.h>
#include "networkdisk.h"
#include <QMessageBox>
#include <QString>

PrivateChat::PrivateChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrivateChat)
{
    ui->setupUi(this);
}

PrivateChat::~PrivateChat()
{
    delete ui;
}

PrivateChat &PrivateChat::getinstance()     //单例模式的私聊对象的
{
    static PrivateChat instance;
    return instance;
}

void PrivateChat::setChatName(QString strName)
{

    m_strChatName=strName;                      //对方名字
    m_strLoginName=Networkdisk::getInstance().loginName();     //我方名字

}

void PrivateChat::updateMsg(const PDU *pdu)
{
    if(NULL==pdu){
        return;
    }
    char caSendName[32]={'\0'};
    memcpy(caSendName,pdu->caData,32);
    QString strMsg=QString("%1 says: %2").arg(caSendName).arg((char*)(pdu->caMsg));
    ui->showMsg_te->append(strMsg);
}

void PrivateChat::on_sendMsg_pb_clicked()
{
    //聊天，首先获取输入框和要私聊的用户的用户名
    QString strMsg=ui->inputMsg_le->text();
    //ui->inputMsg_le->clear();
    if(!strMsg.isEmpty()){
        PDU *pdu=mkPDU(strMsg.size()+1);
        pdu->uiMsgType=ENUM_MSG_TYPE_PRIVATE_REQUEST;
        memcpy(pdu->caData,m_strLoginName.toStdString().c_str(),32);    //当前登录者的名
        memcpy(pdu->caData+32,m_strChatName.toStdString().c_str(),32);     //要私聊对方的名
        //strcpy(pdu->caMsg,strMsg.toStdString().c_str());
        strcpy((char *)(pdu->caMsg),strMsg.toStdString().c_str());
        Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);
        free(pdu);
        pdu=NULL;
    }
    else{
        QMessageBox::information(NULL,"私聊","发送的聊天信息不能为空");
    }
}

