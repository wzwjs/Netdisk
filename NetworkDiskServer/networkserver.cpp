#include "networkserver.h"
#include "ui_networkserver.h"


NetworkServer::NetworkServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NetworkServer)
{
    ui->setupUi(this);
    loadConfig();

    //这个老师的示例是将有关QTcpServer的东西尽量的封装成一个类，然后就只调用即可,即封装成为了一个单例模式的类
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);

    /*我自己的写法，不基于单例模式的QTcpServer*/
    /*
        第一步，就是在头文件里面声明QTcpServer这个类，以及声明一个QTcpServer类对象，并在其cpp文件里面初始化
        第二步，就是调用监听，QTcpServer这个类负责监听，连接等功能。
        第三步，就是QTcpSocket以及其派生类，负责和客户端数据交互
    */

    //今天下午，10月4日
        /*
            自定义了一个协议，结构体，传输的时候是以这个结构体传输的

            将QTcpSocket抽象成为一个类，应对于不同的客户端连接。

    */

    /*
        今天上午，10月5日上午
        实现在服务端单例模式操作数据库，定义枚举自定义数据的类型
        并实习了通过自定义协议封装的数据发送与接收，并与数据库响应


        疑问，QString data=QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
             QString data=QString("insert into usrInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);  //用名字替换%1,用密码替换%2
             data.toStdString().c_str(); 这个意思


        下午，在处理客户端下线的操作时，在服务端因为要根据当前客户端的name和pwd的值去修改对应的客户端状态的操作
             但是，在mytcpsocket.cpp中的套接字接受对应的值的时候，只是在一个函数内部接收的值，是局部变量，搞不了。
             目前有三种解决方法，一种是有不有方法，将这个接收的值全局化，第二种是将这个值当作为函数的返回值传递出来
             第三种是用另一种方法去获得客户端中的pwd和name

            这个老师解决这个的办法是，在mytcpsocket.cpp中接收数据的时候，就用一个变量将这个客户端的名字的值给接下。
            因为这个变量是全局变量，所以就可以用这个变量去对应的数据，去修改对应的数据   ------其实这也是我之前想法的第一个的具体实现


            疑问，"update usrInfo set online=0 where name=\'%1\'"为什么这里的还要加\\是什么作用？
            今天下午的任务是完成客户端下线后的数据库状态的改变和对应QLIST里面的socket的删除     ，今天晚上还要回顾一下这个具体的过程

        10月10日的下午
            主要搞的是添加好友请求，首先是客户端点击加好友的请求，在对应的槽函数里面获取对应的名字，然后将名字封装到协议单元里面，再将这协议单元发送给服务器。
            服务器接收协议数据单元，首先是向数据库里面查询这两个协议数据单元的关系，如果query.next()==true则返回0出去，表示该好友已经是当前用户的好友
            如果不是当前用户的好友，进入else语句体内(好友在线，好友存在且不在线，好友不存在，好友名跟登录用户名同名等等情况)

            服务端在对这一些情况，分别进行响应给客户端，以函数返回数字的形式来分类处理各种情况

            -1，代表的是未知错误
            0,代表的是该好友已经存在
            1，代表存在且Online
            2,代表存在但不在线
            3，代表不存在

            对于非1的请求，都是直接封装对应的回复消息，回复给客户端即可
            对于1的请求，则应该先向对应的客户端转发来自当前客户端的请求
            如果对应客户端同意，即会发送一个回复给当前客户端，并在数据库在添加其两者好友信息。
            如果不同意，就发送一个不同意的回复给当前的客户端

            在客户端这里写一个接收回复的消息，如果同意，就回复同意，并在数据库中的好友关系表里面添加这两个用户之间的关系
            不然，就被添加用户就回复不同意给添加者用户



            *********这个老师讲的逻辑里面，在请求的查询的这个用户不是登录用户的好友这里少了一个逻辑判断，导致在当前用户和请求用户同名的时候，服务端程序没有对应的处理逻辑，从而就会出现程序崩溃*********
            *********根据这个老师写的逻辑，不知道是我跟着他写错误了，还是怎么了，在一个登录用户添加另一个不是好友列表的用户，就出现了服务器崩溃的事情。
            *********在添加好友的这个逻辑这里，中间的逻辑断了，晚上抽空完成*******

        10，15   在处理群聊的时候，在客户端在friend类里面用单例模式就在其他类networkdisk.cpp里面调用的时候，就没有对应的效果，用opewidget里面单例模式就可以
                 在服务器，我的对这个群聊的响应，为什么不行，是因为我的没有对这一些在线好友进行，而是对所有的好友进行。

                针对于这个Friend类里面用单例模型在其他类里面调用的时候，调用不了的原因是Friend类实例化的对象是opewidget类的子对象的布局。在布局中，要操控小布局，就要先从大布局开始。

                但是，对于这个第二个问题，我的对所有在线用户的响应为什么不行。应该是架构问题，我目前也搞不懂。

                还有接收有时候添加好友会出现错误，问题就出在于数据库操作这里，
*/

    // M_SERVER=new QTcpServer(this);    //初始化QTcpServer
    // M_SERVER->listen(QHostAddress(m_strIP),m_usPort);
    //connect(&MyTcpServer::getInstance(),&QTcpServer::newConnection,this,&NetworkServer::do_newConnect);
    // clientSocket = Server->nextPendingConnection(); // 获取客户端连接
    // connect(clientSocket, &QTcpSocket::readyRead, this, &Widget::readClientData);
    // s_socket=MyTcpServer::getInstance().nextPendingConnection();



}

NetworkServer::~NetworkServer()
{
    delete ui;
}

void NetworkServer::loadConfig()
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
            //qDebug()<<m_strIP<<"   "<<m_usPort;
        }
    }
    else{
        QMessageBox::critical(this,"读取配置文件","读取配置文件失败");
    }
}

// void NetworkServer::do_newConnect()
// {
//      qDebug()<<"服务器接受了来自客户端连接";
//     // clientSocket = Server->nextPendingConnection(); // 获取客户端连接
//     s_socket=MyTcpServer::getInstance().nextPendingConnection();
//     connect(s_socket, &QTcpSocket::readyRead, this, &NetworkServer::readClientData);

// }

// void NetworkServer::readClientData()
// {   QByteArray data;
//     data=s_socket->readAll();
//     // ui->textEdit->append("收到数据："+data);
// }

