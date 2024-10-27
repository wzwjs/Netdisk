#include "online.h"
#include "ui_online.h"
#include <QDebug>
#include "protocol.h"
#include <stdlib.h>
#include <unistd.h>
#include "networkdisk.h"

Online::Online(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Online)
{
    ui->setupUi(this);

    // 连接信号槽
    //QObject::connect(listWidget, &QListWidget::itemClicked, onItemClicked);
}

Online::~Online()
{
    delete ui;
}

void Online::showUsr(PDU *pdu)
{
    if(pdu==NULL)
    {
        return;
    }
    //提取使用服务端发的用户名
    //每32位就拷贝出来
    uint uiSize=(pdu->uiMsgLen)/32;//个数
    char caTmp[32];
    for(int i=0;i<uiSize;i++){
        memcpy(caTmp,(char *)(pdu->caMsg)+i*32,32);
        ui->listWidget->addItem(caTmp);
    }

}

void Online::on_addFriend_pb_clicked()
{
    //向服务端发送添加好友的请求
    //获取当前选定的用户的名字
    //QString
    QString name=ui->listWidget->currentItem()->text();
    QString login_name=Networkdisk::getInstance().loginName();
    //qDebug()<<name;
    // if(name==login_name){
    //     QMessageBox::warning();
    // }
    PDU *pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    strncpy(pdu->caData,name.toStdString().c_str(),32);
    strncpy(pdu->caData+32,login_name.toStdString().c_str(),32); //传登录的名字
    Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);
    //再向服务端发送请求
    free(pdu);
    pdu=NULL;
}

