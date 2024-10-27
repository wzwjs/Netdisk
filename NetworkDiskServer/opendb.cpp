#include "opendb.h"
#include<QMessageBox>
#include <QDebug>
#include<QFile>

OpenDB::OpenDB(QObject *parent)
    : QObject{parent}
{
    m_db=QSqlDatabase::addDatabase("QSQLITE");

    init();
}

OpenDB &OpenDB::getInstance()   //这个&OpenDB是什么意思，为什么OpenDB前面还要加上&还要
{
    static OpenDB instance;
    return instance;         //单例模式操作数据库
}

void OpenDB::init()
{

    m_db.setHostName("localhost");//连接数据库的地址
    //这样直接将数据库文件放到资源文件里面，打开不了   ,直接要将数据库路径写进来即可
    m_db.setDatabaseName("E:\\Qt_Project\\Qt_program\\myself\\Netdisk\\cloud.db");

    if(m_db.open())
    {

        QSqlQuery query;
        query.exec("select * from usrInfo");
        while (query.next()) {
            //qDebug()<<"运行到了m_db.open()里面的while里面";
            QString data=QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
            //qDebug()<<data;
        }
    }
    else{
        QMessageBox::critical(NULL,"打开数据库","打开数据库失败");
    }

}

OpenDB::~OpenDB()
{
    m_db.close();//在OpenDB回收的时候，关闭数据库
}

bool OpenDB::handleRegist(const char *name, const char *pwd)
{//往数据库查询里面查询对应的数据，所以要拼接一个数组
    // char caQuery[128]={'\0'};
    // sprintf(caQuery,"");
    if(NULL==name ||NULL==pwd){
        qDebug()<<"name \pwd is null";
        return false;
    }                                       //考虑形式参数的有效性
    QString data=QString("insert into usrInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);  //用名字替换%1,用密码替换%2
    //qDebug()<<data;
    QSqlQuery query;
    return  query.exec(data);
}

bool OpenDB::handleLogin(const char *name, const char *pwd)
{
    if(NULL==name ||NULL==pwd){
        qDebug()<<"name \pwd is null";
        return false;
    }  //考虑形式参数的有效性

    //在数据库里面查询是否有登录者的信息
/*
 *     自己写的数据库查询语句
 *     QSqlQuery query;
    // // 使用占位符，并通过 arg() 方法替换变量
    // QString query = QString("SELECT * FROM usrInfo WHERE name='%1' AND pwd='%2'")
    //                     .arg(name)
    //                     .arg(pwd);
    query.prepare("select name,pwd from usrInfo where name==? and pwd==? and online=0");
    query.bindValue(0, name);
    query.bindValue(1, pwd);


    return query.exec();
*/

    QString data=QString("select * from usrInfo where name=\'%1\' and pwd=\'%2\' and online=0").arg(name).arg(pwd);  //用名字替换%1,用密码替换%2

    QSqlQuery query;
    query.exec(data);



    //登录成功之后对数据库的数据进行修改，就是将online的值改变为1
    if(query.next()){
        QString data=QString("update usrInfo set online=1 where name=\'%1\' and pwd=\'%2\'").arg(name).arg(pwd);  //用名字替换%1,用密码替换%2

        QSqlQuery query;
        query.exec(data);
        return true;
    }
    else{
        return false;
    }

}

void OpenDB::handleDownLine(const char *name)
{
    if(NULL==name ){
        qDebug()<<"name is null";
        return ;
    }  //考虑形式参数的有效性
    QString data=QString("update usrInfo set online=0 where name=\'%1\'").arg(name);  //用名字替换%1,用密码替换%2
    QSqlQuery query;

    if(query.exec(data)){
        QMessageBox::information(NULL,"客户端下线","当前客户端以及下线成功");
    }
    else{
        QMessageBox::warning(NULL,"客户端下线","当前客户端以及下线失败");
    }
}

QStringList OpenDB::handleAllOnlineUser()
{
    //返回的是所有的名字，就是字符串列表
    QString data=QString("select name from usrInfo where  online=1");
    QSqlQuery query;
    query.exec(data);
    QStringList result;
    result.clear();
    while (query.next()) {
        result.append(query.value(0).toString());
    }
    return result;
}

int OpenDB::handleAddFriend(const char *his_Name, const char *my_Name)
{
    //主要处理的逻辑是
    if(NULL==his_Name||NULL==my_Name){
        qDebug()<<"name or his_Name is null";
        return -1;   //-1表示系统故障，添加好友失败
    }  //考虑形式参数的有效性
    QString data=QString("select * from friend where "
                           "(id=(select id from usrInfo where name=\'%1\') and "
                           "friendId=(select id from usrInfo where name=\'%2\'))"
                           " or (friendId=(select id from usrInfo where name='\%3\')"
                           " and id=(select id from usrInfo where name='\%4\'))").arg(his_Name).arg(my_Name).arg(my_Name).arg(his_Name);

    //qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    //qDebug()<<query.next();
    if(query.next()){
        return 0;//该好友已经存在
    }
    else//该else表示的是该用户不是当前用户的好友
    {
        //qDebug()<<"this is falut point";
        //判断对方是否在线
        QString data=QString("select online from usrInfo where name=\'%1\' ").arg(his_Name);  //用名字替换%1,

        QSqlQuery query;
        query.exec(data);
        if(query.next()){
            int ret=query.value(0).toInt();
            if(ret==1){
                //存在且online
                return 1;
            }
            else if(ret==0){
                return 2;//存在但不在线
            }
        }
        else{
            //不存在
            return 3;
        }
    }
}

int OpenDB::handleReacherUsr(const char *name)
{
    if(NULL==name){
        qDebug()<<"name  is null";
        return -1;
    }  //考虑形式参数的有效性

    QString data=QString("select online from usrInfo where name=\'%1\' ").arg(name);  //用名字替换%1,

    QSqlQuery query;
    query.exec(data);
    if(query.next()){
        int ret=query.value(0).toInt();
        if(ret==1){
            //存在且online
            return 1;
        }
        else if(ret==0){
            return 0;//存在但不在线
        }
    }
    else{
        //不存在
        return -1;
    }
}

QStringList OpenDB::handleFlushFriend(const char *name)
{
    QStringList strFriendList;
    strFriendList.clear();
    if(NULL==name){
        return strFriendList;
    }

    //AS friend
    //QString data = QString("select id from userInfo where name = \'%1\' and online = 1 ").arg(name);
    QString data=QString("select name from usrInfo where online=1 and id in (select id from friend where friendId=(select id from usrInfo where name=\'%1\')) ").arg(name);  //用名字替换%1,
    QSqlQuery query;
    query.exec(data);
    //int sourceId = -1; // 请求方name对应的id
    while (query.next()) {
        strFriendList.append( query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }
    // if (query.next())
    // {
    //     sourceId = query.value(0).toInt();
    // }

    // return strFriendList;

    //AS friend
    data=QString("select name from usrInfo where online=1 and id=(select friendId from friend where id=(select id from usrInfo where=\'%1\')) ").arg(name);  //用名字替换%1,
    //data = QString("select name, online from userInfo where id in (select id from friend where friendId= \'%1\') or id in (select friendId from friend where id = \'%2\')").arg(sourceId).arg(sourceId);
    query.exec(data);
    while (query.next()) {
        strFriendList.append(query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }
    return strFriendList;
}

bool OpenDB::handleDelete(const char *my_Name, const char *his_Name)
{
    if(NULL==my_Name||NULL== his_Name){
        qDebug()<<"name  is null";
        return false;
    }  //考虑形式参数的有效性
    //删除好友
    QString my_name=my_Name;
    QString his_name=his_Name;

    QString data;
    QSqlQuery query;


    data=QString(" delete  from friend where id =(select id from usrInfo where name=\'%1\') and friendId=(select id from usrInfo where name=\'%2\')").arg(my_name).arg(his_Name);
    query.exec(data);
    data=QString(" delete  from friend where id =(select id from usrInfo where name=\'%1\') and friendId=(select id from usrInfo where name=\'%2\')").arg(his_Name).arg(my_name);


    query.exec(data);


    return  true;
}



// bool OpenDB::handleReacher(const char *name)
// {
//     if(NULL==name ){
//         qDebug()<<"name  is null";
//         return false;
//     }  //考虑形式参数的有效性

//     QString data=QString("select * from usrInfo where name=\'%1\' ").arg(name);  //用名字替换%1,用密码替换%2

//     QSqlQuery query;
//     return query.exec(data);
// }
