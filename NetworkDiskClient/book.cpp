#include "book.h"
#include "networkdisk.h"
#include <QInputDialog>
#include<QMessageBox>

Book::Book(QWidget *parent)
    : QWidget{parent}
{
    m_strEnterDir.clear();//一开始的时候，将其清空
    //返回
    m_reback=new QPushButton("返回");
    //文件夹
    m_create_files=new QPushButton("创建文件夹");
    m_delete_files=new QPushButton("删除文件夹");
    m_reName_files=new QPushButton("重命名文件夹");
    m_flush_files=new QPushButton("刷新文件夹");

    //文件
    m_up_file=new QPushButton("上传文件");
    m_down_file=new QPushButton("下载文件");
    m_delete_file=new QPushButton("删除文件");
    m_share_file=new QPushButton("共享文件");

    //列表
    m_pFileListWidget=new QListWidget(this);

    QVBoxLayout *vb1=new QVBoxLayout;
    QVBoxLayout *vb2=new QVBoxLayout;
    QVBoxLayout *vb3=new QVBoxLayout;

    vb1->addWidget(m_pFileListWidget);

    vb2->addWidget(m_reback);
    vb2->addWidget(m_create_files);
    vb2->addWidget(m_delete_files);
    vb2->addWidget(m_reName_files);
    vb2->addWidget(m_flush_files);

    vb3->addWidget(m_up_file);
    vb3->addWidget(m_down_file);
    vb3->addWidget(m_delete_file);
    vb3->addWidget(m_share_file);

    QHBoxLayout *HB=new QHBoxLayout;
    HB->addLayout(vb1);
    HB->addLayout(vb2);
    HB->addLayout(vb3);


    setLayout(HB);
    //创建文件夹的槽函数
    connect(m_create_files,SIGNAL(clicked(bool)),this,SLOT(create_files()));
    //刷新文件夹
    connect(m_flush_files,SIGNAL(clicked(bool)),this,SLOT(flushFile()));
    //删除文件夹
    connect(m_delete_files,SIGNAL(clicked(bool)),this,SLOT(delete_Dir()));
    //重命名文件夹
    connect(m_reName_files,SIGNAL(clicked(bool)),this,SLOT(rename_Files()));
    //进入文件夹
    connect(m_pFileListWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(enter_Dir(QModelIndex)));
}

void Book::updateFileList( PDU *pdu)
{
     //qDebug()<<"run this 5";
    //刷新文件列表
    if(NULL==pdu){
        return;
    }
    // QListWidgetItem *pItemTmp=NULL;
    //m_pBookListWidget->clear();
    // int row=m_pBookListWidget->count();
    // for(int i=0;i<row;i++){
    //     pItemTmp=m_pBookListWidget->item(row-1);
    //     m_pBookListWidget->removeItemWidget(pItemTmp);
    //     delete pItemTmp;
    //     row=row-1;
    // }
    //qDebug()<<"run this 6";
    FileInfo *pFileInfo=NULL;
    int iCount=(pdu->uiMsgLen)/sizeof(FileInfo);//总的消息大小
     //m_pBookListWidget->clear();10 月24日第四个问题答案
    m_pFileListWidget->clear();
    for(int i=0;i<iCount;i++){
       //qDebug()<<"run this 7";
        //pFileInfo=(FileInfo*)(pdu->caMsg);  //10月24日  问题2
        pFileInfo=(FileInfo*)(pdu->caMsg)+i;
        //qDebug()<<pFileInfo->caFileName<<pFileInfo->iFileType;
        QListWidgetItem *pItem=new QListWidgetItem;
        if(0==pFileInfo->iFileType){
            //目录
             //qDebug()<<"run this 8";
            pItem->setIcon(QIcon(QPixmap(":/map/dir.png")));
        }
        else if(1==pFileInfo->iFileType){
            //常规文件
            pItem->setIcon(QIcon(QPixmap(":/map/regula_file.png")));
        }
        pItem->setText(pFileInfo->caFileName);

        //10月24日  第五个问题  m_pBookListWidget->addItem(pItem);    应该就是这个添加项的容器搞错了，从而导致搞不了
        m_pFileListWidget->addItem(pItem);
        //qDebug()<<"run this 8";
    }
    //qDebug()<<"run this 9";
    return ;  //问题3 10月24日，这里没有返回
}

void Book::create_files()
{
    //这里创建文件的逻辑是，在默认注册的时候，在服务器端就为每一个用户以用户名为文件名，创建了一个文件夹，后续在该用户创建一个新的文件时，默认放到该路径下
    //要创建文件，就要告诉服务器，在那里创建文件，即当前文件路径，和新文件的名字和当前的用户名，将其发送给服务器
    QString strNewDir=QInputDialog::getText(this,"新建文件夹","新文件夹名字");
    if(!strNewDir.isEmpty()){
        if(!(strNewDir.size()>32)){
            QString login_name=Networkdisk::getInstance().loginName();    //当前登录的名字
            QString strCurPath=Networkdisk::getInstance().CurPath();        //当前的路径
            PDU *pdu=mkPDU(strCurPath.size()+1);
            pdu->uiMsgType=ENUM_MSG_TYPE_CREATE_PATH_REQUEST;
            strncpy(pdu->caData,login_name.toStdString().c_str(),32);//名字
            strncpy(pdu->caData+32,strNewDir.toStdString().c_str(),32);//路径  }  //新文件夹的名字不能超过32位
            memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());//当前路径
            Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);
            free(pdu);
            pdu=NULL;
        }else{
            QMessageBox::warning(this,"输入路径","输入路径超过了32个字符，请输入32个字符以下的路径");
        }
    }
    else{
        QMessageBox::warning(this,"新建文件夹","新文件夹不能为空");
    }

}

//刷新文件
void Book::flushFile()
{
    //向服务器请求查看当前路径下的文件列表

    //需要将当前路径发给服务器，和当前用户名
    //QString login_name=Networkdisk::getInstance().loginName();    //当前登录的名字
    QString strCurPath=Networkdisk::getInstance().CurPath();        //当前的路径
    PDU *pdu=mkPDU(strCurPath.size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILES_REQUEST;//刷新文件夹请求
    //strncpy(pdu->caData,login_name.toStdString().c_str(),32);//名字
    memcpy((char*)(pdu->caMsg),strCurPath.toStdString().c_str(),strCurPath.size());//当前路径
    Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);
    free(pdu);
    pdu=NULL;
}

void Book::delete_Dir()
{
    //获取当前所在的目录
    QString strCurPath=Networkdisk::getInstance().CurPath();        //当前的路径
    QListWidgetItem *pItem= m_pFileListWidget->currentItem();       //获取当前选中的文件
    if(NULL==pItem){
        return ;//当前并未选中文件夹
        QMessageBox::warning(this,"删除文件","请选择要删除的文件");
    }
    else{
        //获取删除文件名
        QString strDelName=pItem->text();
        PDU *pdu=mkPDU(strCurPath.size()+1);
        pdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_REQUEST;
        memcpy(pdu->caData,strDelName.toStdString().c_str(),strDelName.size());  //要删除的文件名
        memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());  //路径
        Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);
        free(pdu);
        pdu=NULL;
    }
}

void Book::rename_Files()
{
  //获取当前目录信息，和要修改的文件名
    //获取当前所在的目录
    QString strCurPath=Networkdisk::getInstance().CurPath();        //当前的路径
    QListWidgetItem *pItem= m_pFileListWidget->currentItem();       //获取当前选中的文件
    if(NULL==pItem){
        return ;//当前并未选中文件夹
        QMessageBox::warning(this,"重命名文件","请选择要重命名的文件");
    }
    else{
        //获取文件名
        QString strOldName=pItem->text();
        QString strNewName=QInputDialog::getText(this,"重命名文件","请输入新的文件名");
        if(!strNewName.isEmpty()){
            PDU *pdu=mkPDU(strCurPath.size()+1);
            pdu->uiMsgType=ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
            memcpy(pdu->caData,strOldName.toStdString().c_str(),strOldName.size());     //要重命名的文件名
            memcpy(pdu->caData+32,strNewName.toStdString().c_str(),strNewName.size());  //重命名后的文件名
            memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());      //路径
            Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);
            free(pdu);
            pdu=NULL;}
        else{
            QMessageBox::warning(this,"重命名文件","新文件名不能为空");
        }


    }
}

void Book::enter_Dir(const QModelIndex &index)
{
    QString strDirName=index.data().toString();             //caData
    m_strEnterDir=strDirName;
    QString strCurPath=Networkdisk::getInstance().CurPath();//caMsg
    qDebug()<<strDirName;
    qDebug()<<strCurPath;
    PDU *pdu=mkPDU(strCurPath.size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_ENTER_DIR_REQUEST;
    memcpy(pdu->caData,strDirName.toStdString().c_str(),strDirName.size());
    memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
    Networkdisk::getInstance().getTcpSocket().write((char *)pdu,pdu->uiPDUlen);
    free(pdu);
    pdu=NULL;

}



QString Book::enterDir()
{
    return m_strEnterDir;
}

void Book::clearEnterDir()
{
    m_strEnterDir.clear();
}
