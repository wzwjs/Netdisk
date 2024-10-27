#include "mytcpsocket.h"
#include <QDebug>
#include "protocol.h"
#include "opendb.h"
#include <QStringList>
#include<QMessageBox>
#include "mytcpserver.h"
#include <QDir>
#include <QFileInfo>

MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));  //本身发送的数据由本身来处理
    connect(this,SIGNAL(disconnected()),this,SLOT(clientOffLine()));  //处理当前客户端下线的请求
}

QString MyTcpSocket::getName()
{
    return m_strName;
}

void MyTcpSocket::recvMsg()
{
    //qDebug()<<this->bytesAvailable();
    //收数据
    uint uiPDUlen=0;                                         //这是在函数内部声明这个协议的所有数据大小
    this->read((char *)&uiPDUlen,sizeof(uint));             //如何将这个协议里面内部的数据大小保存到这个uiPDUlen内
    uint uiMsgLen=uiPDUlen-sizeof(PDU);//实际消息的长度
    PDU *pdu=mkPDU(uiMsgLen);  //利用这个PDU去接收剩余的数据
    this->read((char *)pdu+sizeof(uint),uiPDUlen-sizeof(uint));
    //qDebug()<<pdu->uiMsgType<<(char *)(pdu->caMsg);           //打印数据类型


    //收到消息之后，判断其的消息类型
    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_REGIST_REQUEST:
        {//如果是注册请求，就将消息搞出来

        //收数据，看数据是否发送过来
        char caName[32]={'\0'};
        char caPwd[32]={'\0'};
        strncpy(caName,pdu->caData,32);         //接收客户端的数据，接收客户端的请求的用户名和密码
        strncpy(caPwd,pdu->caData+32,32);

        //与数据库交互
        bool ret= OpenDB::getInstance().handleRegist(caName,caPwd);
        PDU *respdu=mkPDU(0);
        respdu->uiMsgType=ENUM_MSG_TYPE_REGIST_RESPOND;//消息类型是注册回复
        if(ret==true){
            strcpy(respdu->caData,REGIST_OK);

            //新用户在注册的时候，就以对应的用户名创建对应的文件夹
            QDir dir;
            bool result=dir.mkdir(QString("./%1").arg(caName));
            if(result){
                QMessageBox::information(NULL,"创建文件夹","服务器创建文件夹成功");
            }
            else{
                QMessageBox::warning(NULL,"创建文件夹","服务器创建文件夹失败");
            }

        }
        else{
            strcpy(respdu->caData,REGIST_FAILED);
        }

        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }

    //如果是登录的请求
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
    {
        //收数据，看数据是否发送过来
        char caName[32]={'\0'};
        char caPwd[32]={'\0'};
        strncpy(caName,pdu->caData,32);         //接收客户端的数据，接收客户端的请求的用户名和密码
        strncpy(caPwd,pdu->caData+32,32);

        //与数据库交互
        bool ret= OpenDB::getInstance().handleLogin(caName,caPwd);
        PDU *respdu=mkPDU(0);
        respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;//消息类型是登录回复
        if(ret==true){
            strcpy(respdu->caData,LOING_OK);
            //登录成功，记录其名字
            m_strName=caName;                //这里登录其名字，有很多作用，因为以及定义了这个名字的属性是唯一的，后面就可以根据这个名字的属性去访问数据库去该对应的操作

        }
        else{
            strcpy(respdu->caData,LOING_FAILED);
        }

        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }
    //处理在线用户请求

    /*
        业务的逻辑是将所有的用户的消息返回出去，就要使用到那个可扩充的协议数据单元
    */
    case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST:
    {
        //查询到所有的用户数据
        QStringList ret=OpenDB::getInstance().handleAllOnlineUser();
        /*测试*/

        char caTmp[32];
        for(int i=0;i<ret.size();i++){
            //qDebug()<<ret[i];

        }


        //将使用的用户数据赋值给协议数据单元
        //qDebug()<<"当前数据库一共有个人:"<<ret.size();
        uint uiMsgLen=ret.size()*32;//计算当前所有用户的字节数
        //qDebug()<<uiMsgLen;
        PDU *respdu=mkPDU(uiMsgLen);

        respdu->uiMsgType=ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;//消息类型是在线用户回复
        //将数据拷贝到协议发送PDU
        for(int i=0;i<ret.size();i++){
            memcpy((char *)(respdu->caMsg)+i*32,ret.at(i).toStdString().c_str(),32);
            //qDebug()<<ret.at(i).toStdString().c_str()<<"   ";
            //qDebug()<<(char *)(respdu->caMsg)+i*32<<"   ";
        }
        //发送处理显示所有在线用户请求的回复
        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }

    //处理查找用户查找的逻辑
    case ENUM_MSG_TYPE_SEARCH_USR_REQUEST:{
        //收数据，看数据是否发送过来
        char caName[32]={'\0'};
        strncpy(caName,pdu->caData,32);
        //qDebug()<<caName;
        //与数据库交互,在数据库中查询该用户是否存在
        int ret= OpenDB::getInstance().handleReacherUsr(caName);
        //qDebug()<<ret;
        PDU *respdu=mkPDU(0);
        respdu->uiMsgType=ENUM_MSG_TYPE_SEARCH_USR_RESPOND;//消息类型是在线用户回复
        if(ret==1){
            //在数据库中查询到了该用户,且在线
            strcpy(respdu->caData,SEARCH_USR_ONLINE);
        }
        else if(ret==0){
            // QMessageBox::information(this,"查找","在当前数据库中找不");
            // QMessageBox::warning(this,"查找","查找失败");
            //qDebug()<<"查找失败";
            //不在线
            strcpy(respdu->caData,SEARCH_USR_OFFLINE);
        }
        else if(ret==-1){
            //不存在
            strcpy(respdu->caData,SEARCH_USR_NO);
        }

        //发送处理查找在线用户请求的回复
        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }

    //处理添加好友的请求
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
        //收数据，看数据是否发送过来
        char his_Name[32]={'\0'};
        char my_Name[32]={'\0'};

        strncpy(his_Name,pdu->caData,32);         //接收客户端的数据，接收客户端的请求的添加好友的姓名和登录者的姓名
        strncpy(my_Name,pdu->caData+32,32);

        //与数据库交互,查询该两者是否为好友
        int ret= OpenDB::getInstance().handleAddFriend(his_Name, my_Name);
        //qDebug()<<"this is falut point";
        PDU *respdu=NULL;
        switch (ret) {                          //这个switch里面可能传入两种消息类型，一种是add_friend_repond另一种是由于原来的pdu里面的add_friend_request
        case -1:
        {
            respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;  //添加好友回复
            memcpy(respdu->caData,UNKNOW_ERROR,32);              //未知错误
            break;
        }

        case 0:{
            respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;  //添加好友回复
            memcpy(respdu->caData,EXITSTED_FRIEND,32);             //该用户已经是好友了
            break;
            //qDebug()<<"this is falut point";
        }

        case 1:{
            MyTcpServer::getInstance().resend(his_Name,pdu);//这里为什么是pdu,而不是respdu
            //这里处理的是目标好友在线，且不是请求用户的好友，可以加为好友

            break;
             //该好友在线
        }

        case 2:{
            respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;  //添加好友回复
            memcpy(respdu->caData,ADD_FRIEND_OFFLINE,32);       //该好友不在线
            break;
        }
        case 3:{
            respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;  //添加好友回复
            memcpy(respdu->caData,ADD_FRIEND_NOEXIST,32);       //该好友添加失败
            break;
        }

        default:
            break;
        }


        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;

    }

    //对于被添加用户同意添加者用户的好友请求，就在好友表里面加一个两个好友之间的关系，最后来，再回显到客户端即可
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:{
        break;
    }
    //对于被添加用户同意添加者用户的好友请求，就在好友表里面加一个两个好友之间的关系，最后来，再回显到客户端即可
    case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:{
        break;
    }

    //刷新好友请求
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:{
        //收数据，看数据是否发送过来
        char caName[32]={'\0'};
        strncpy(caName,pdu->caData,32);
        QStringList ret= OpenDB::getInstance().handleFlushFriend(caName);
        uint uiMsgLen=ret.size()*32;
        PDU *respdu=mkPDU(uiMsgLen);
        respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
        //循环的将名字拷贝到该pdu
        for(int i=0;i<ret.size();i++){
            memcpy((char *)(respdu->caMsg)+i*32,
                   ret.at(i).toStdString().c_str(),
                   ret.at(i).size());
        }
        //qDebug()<<"run this";
        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }
    //删除好友请求处理
    case  ENUM_MSG_TYPE_DELETE_REQUEST:{

         //服务器接收，并在数据库中删除好友，并将新的好友列表返回给客户端
        //收数据，看数据是否发送过来
        char his_Name[32]={'\0'};
        char my_Name[32]={'\0'};
        memcpy(my_Name,pdu->caData,32);     //当前登录用户的用户名
        memcpy(his_Name,pdu->caData,32);    //获取当前选中要删除用户的用户名
        OpenDB::getInstance().handleDelete(my_Name,his_Name);

        PDU *respdu=mkPDU(0);
        respdu->uiMsgType=ENUM_MSG_TYPE_DELETE_RESPOND;
        strcpy(respdu->caData,DELETE_FRIEND_OK);
        //qDebug()<<"run this";
        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;

        MyTcpServer::getInstance().resend(his_Name,pdu);   //给要删除好友的回复


        break;

    }
    //私聊好友的请求
    case  ENUM_MSG_TYPE_PRIVATE_REQUEST:{
        //找到对方的socket，再转发信息
        char hisname[]={'\0'};//对方的名
        memcpy(hisname,pdu->caData+32,32);
        qDebug()<<hisname;
        MyTcpServer::getInstance().resend(hisname,pdu);//转发给对方
        break;
    }
    //群聊好友
    case  ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:{
        //获取发送方的名字，然后获取所有在线的用户
        char caName[32]={'\0'};
        memcpy(caName,pdu->caData,32);
        QStringList online_friend= OpenDB::getInstance().handleFlushFriend(caName);


        //接收消息
       // // qDebug()<<"1， do you run this ?";
       //  char msg[]={'\0'};
       //  memcpy(msg,pdu->caMsg,pdu->uiMsgLen);
       //  //qDebug()<<msg;
       //  //查询到所有的用户数据
       //  QStringList ret=OpenDB::getInstance().handleAllOnlineUser();
        for(int i=0;i<online_friend.size();i++){
            //将这个pdu发送给这个Socket List里面的任何一个Socket
            MyTcpServer::getInstance().resend(online_friend.at(i).toStdString().c_str(),pdu);
        }
        qDebug()<<"2， do you run this ?";
        break;
    }
    //创建文件夹
    case ENUM_MSG_TYPE_CREATE_PATH_REQUEST:{
        QDir dir;
        QString strCurPath=QString("%1").arg((char *)(pdu->caMsg));
        //qDebug()<<strCurPath;
        PDU * respdu;
        bool ret=dir.exists(strCurPath);
        if(ret){        //当前目录存在
            char caNewDir[32]={'\0'};
            memcpy(caNewDir,pdu->caData+32,32);//新文件夹的名字
            QString strNewPath=strCurPath+"/"+caNewDir;

            //dir.mkdir(strNewPath);
            ret=dir.exists(strNewPath);
            qDebug()<<"--->"<<ret;
            if(ret){        //创建的文件名已经存在
                respdu=mkPDU(0);
                respdu->uiMsgType=ENUM_MSG_TYPE_CREATE_PATH_RESPOND;
                strcpy(respdu->caData,FILE_NAME_EXIST);
            }
            else{

                dir.mkdir(strNewPath);
                //创建的文件名不存在
                respdu=mkPDU(0);
                respdu->uiMsgType=ENUM_MSG_TYPE_CREATE_PATH_RESPOND;
                strcpy(respdu->caData,CREAT_DIR_OK);
            }
        }else{
            //当前路径不存在
            respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_CREATE_PATH_RESPOND;
            strcpy(respdu->caData,DIR_NO_EXIST);

        }
        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }
    //客户端刷新  当前文件夹的请求
    case ENUM_MSG_TYPE_FLUSH_FILES_REQUEST:{
        //问题是怎么拿到当前路径下的所有的文件名
        //QDir类里面有相关的成员函数，就直接让该对象调用即可
        //qDebug()<<"run this 1";
        char *pCurPath=new char[pdu->uiMsgLen];//路径的大小
        memcpy(pCurPath,pdu->caMsg,pdu->uiMsgLen);//将路径拷贝到pCurPath中
        QDir dir(pCurPath);                        //构造当前目录
        QFileInfoList fileinfoList=dir.entryInfoList(); //遍历得到当前路径下的文件的信息
        int iFilecount=fileinfoList.size();
        //qDebug()<<"run this 2";
        //申请空间
        //问题1，PDU *respdu=mkPDU(sizeof(QFileInfo)*iFilecount);
        PDU *respdu=mkPDU(sizeof(FileInfo)*(iFilecount));
        respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILES_RESPOND;//刷新文件的回复
        FileInfo *pFileInfo=NULL;       //文件；类型结构体的指针
        QString strFileName;
        //qDebug()<<"当前路径下一共有"<<ifilecount<<"文件";
        for(int i=0;i<fileinfoList.size();i++){
            pFileInfo=(FileInfo*)(respdu->caMsg)+i;
            strFileName=fileinfoList[i].fileName();
            memcpy(pFileInfo->caFileName,strFileName.toStdString().c_str(),strFileName.size());
            if(fileinfoList[i].isDir()){
                //如果路径为真，就设置为0，0表示为一个文件夹
                //qDebug()<<"run this 3";
                pFileInfo->iFileType=0;
            }
            else if(fileinfoList[i].isFile()){
                //qDebug()<<"run this 4";
                pFileInfo->iFileType=1; //常规文件
            }


            //tem+=fileinfoList[i].fileName().size();
            // qDebug()<<fileinfoList[i].fileName().size()
            //     <<"文件夹:"<<fileinfoList[i].isDir()
            //     <<"常规文件："   <<fileinfoList[i].isFile();

        }
        // //将fileinfoList的filename返回给客户端，客户端解析到界面上即可
        // //思路是，获取所有的名字，将这些名字返回到客户端，
        // int tem=0,ifilecount1=0;
        // ifilecount=fileinfoList.count();
        // PDU *respdu=mkPDU(tem+ifilecount);
        // PDU *respdu=mkPDU(sizeof(FileInfo)*ifilecount);
        // //PDU *respdu=mkPDU()
        // respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILES_RESPOND;
        // for(int i=0;i<fileinfoList.size();i++){
        //     strcpy((char *)(respdu->caMsg),fileinfoList[i].fileName().toStdString().c_str());
        // }
        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }
    //删除目录的请求
    case ENUM_MSG_TYPE_DEL_DIR_REQUEST:{
        //拼接路径
        char caName[32]={'\0'};
        strcpy(caName,pdu->caData);         //拷贝要删除的文件的名字
        char *pPath=new char[pdu->uiMsgLen];//路径的长度
        memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);//拷贝路径
        QString strPath=QString ("%1/%2").arg(pPath).arg(caName);
        qDebug()<<"当前路径为："<<strPath;

        //删除路径，首先要判断该路径是否为目录，常规文件不是目录，常规文件和目录的删除的方式不一样
        QFileInfo fileInfo(strPath);
        bool ret=false;
        if(fileInfo.isDir()){//如果是目录，即文件夹,文件夹的删除，递归删除该文件夹内部的所有文件
            QDir dir;
            dir.setPath(strPath);
            ret=dir.removeRecursively();
        }
        else if(fileInfo.isFile()){
            //常规文件
            ret=false;
        }
        PDU *respdu=NULL;
        if(ret){  //删除成功
            respdu=mkPDU(strlen(DEL_DIR_OK)+1);
            respdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_RESPOND;
            memcpy(respdu->caData,DEL_DIR_OK,strlen(DEL_DIR_OK));
        }
        else{
            //删除失败
            respdu=mkPDU(strlen(DEL_DIR_FAILURE)+1);
            respdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_RESPOND;
            memcpy(respdu->caData,DEL_DIR_OK,strlen(DEL_DIR_FAILURE));
        }
        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }
    //重命名文件
    case ENUM_MSG_TYPE_RENAME_FILE_REQUEST:{
        //在服务端接收了来自客户端的重命名文件的请求之后，在具体如何重命名文件

        //拼接路径
        char caOldName[32]={'\0'};
        char caNewName[32]={'\0'};
        strncpy(caOldName,pdu->caData,32);         //拷贝要删除的文件的名字
        strncpy(caNewName,pdu->caData+32,32);      //新名字
        char *pPath=new char[pdu->uiMsgLen];        //存放当前的路径的长度
        memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);     //拷贝路径
        QString strOldPath=QString ("%1/%2").arg(pPath).arg(caOldName);//老的名字的路径
        QString strNewPath=QString ("%1/%2").arg(pPath).arg(caNewName);//老的名字的路径


        QDir dir;
        bool ret= dir.rename(strOldPath,strNewPath);
        PDU *respdu=mkPDU(0);
        if(ret){
            //重命名文件成功
            respdu->uiMsgType=ENUM_MSG_TYPE_RENAME_FILE_RESPOND;
            memcpy(respdu->caData,RENAMR_FILE_OK,strlen(RENAMR_FILE_OK));

        }
        else{
            //重命名文件失败
            respdu->uiMsgType=ENUM_MSG_TYPE_RENAME_FILE_RESPOND;
            memcpy(respdu->caData,RENAMR_FILE_FAILE,strlen(RENAMR_FILE_FAILE));
        }
        write((char *)respdu,respdu->uiPDUlen);
        free(respdu);
        respdu=NULL;
        break;
    }
    //进入文件夹的请求
    case ENUM_MSG_TYPE_ENTER_DIR_REQUEST:{
     //1,拼接路径
        //拼接路径
        char EnterName[32]={'\0'};

        memcpy(EnterName,pdu->caData,32);

        char *pPath=new char[pdu->uiMsgLen];        //存放当前的路径的长度
        memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);     //拷贝路径
        QString Path=QString("%1/%2").arg(pPath).arg(EnterName);

        qDebug()<<Path;
    //2,判断路径是否为文件夹
        PDU *respdu=NULL;
        QFileInfo fileinfo(Path);
        if(fileinfo.isDir()){
            //如果是文件夹的话，即将该文件夹内部的文件信息全部遍历出来，封装到pdu里面
            //怎么将文件夹内部的信息封装到pdu里面，然后去转发？
            // respdu=mkPDU(Path.size());
            // respdu->uiMsgType=ENUM_MSG_TYPE_ENTER_DIR_RESPOND;

            QDir dir(Path);                                  //构造当前目录
            QFileInfoList fileinfoList=dir.entryInfoList();     //遍历得到当前路径下的文件的信息
            int iFilecount=fileinfoList.size();                 //文件个数
            //qDebug()<<"run this 2";
            //申请空间
            //问题1，PDU *respdu=mkPDU(sizeof(QFileInfo)*iFilecount);
            PDU *respdu=mkPDU(sizeof(FileInfo)*(iFilecount));    //根据文件的个数，产生对应的空间
            respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILES_RESPOND; //刷新文件的回复
            FileInfo *pFileInfo=NULL;       //文件；类型结构体的指针
            QString strFileName;
            //qDebug()<<"当前路径下一共有"<<ifilecount<<"文件";
            for(int i=0;i<fileinfoList.size();i++){    //循环的从fileinfo列表里面把文件拿出来，再给文件放进去
                pFileInfo=(FileInfo*)(respdu->caMsg)+i;
                strFileName=fileinfoList[i].fileName();
                memcpy(pFileInfo->caFileName,strFileName.toStdString().c_str(),strFileName.size());
                if(fileinfoList[i].isDir()){
                    //如果路径为真，就设置为0，0表示为一个文件夹
                    //qDebug()<<"run this 3";
                    pFileInfo->iFileType=0;
                }
                else if(fileinfoList[i].isFile()){
                    //qDebug()<<"run this 4";
                    pFileInfo->iFileType=1; //常规文件
                }
            }
            write((char *)respdu,respdu->uiPDUlen);
            free(respdu);
            respdu=NULL;
        }
        else if(fileinfo.isFile()){
            //回复进入失败
            respdu->uiMsgType=ENUM_MSG_TYPE_ENTER_DIR_RESPOND;
            memcpy(respdu->caData,ENTER_DIR_FAILED,strlen(ENTER_DIR_FAILED));
            write((char *)respdu,respdu->uiPDUlen);
            free(respdu);
            respdu=NULL;
        }
    //3，是则将文件夹内部的信息都封装到pdu回复成功过去，不是则回复失败给客户端
        break;
    }
    default:
        break;
    }

    free(pdu);
    pdu=NULL;



    //qDebug()<<"接收的用户名是:"<<caName<<"接收的密码是："<<caPwd<<"协议是："<<pdu->uiMsgType;
}

void MyTcpSocket::clientOffLine()
{
    //处理客户端下线的函数
    //将数据库的状态改变成为1
    OpenDB::getInstance().handleDownLine(m_strName.toStdString().c_str());

    //从链表里面删除一个对应的QTcpSocket
    emit offLine(this);     //这个this是什么执指向的是什么？  this指针指向的是当前调用的对象，即当前实例化的QTcpSocket
}


