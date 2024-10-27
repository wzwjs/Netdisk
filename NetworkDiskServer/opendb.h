#ifndef OPENDB_H
#define OPENDB_H

#include <QObject>
#include <QSqlDatabase>    //连接数据库
#include <QSqlQuery>        //查询数据库
#include <QStringList>      //去保存字符串列表的名字

class OpenDB : public QObject
{
    Q_OBJECT
public:
    explicit OpenDB(QObject *parent = nullptr);

    //单例模式
    static OpenDB& getInstance();  //定义一个静态的成员函数    这个&在这里是什么意思？
    void init();//封装连接数据库为初始化函数
    ~OpenDB();
    bool handleRegist(const char *name,const char * pwd);  //处理来自客户端的注册请求
    bool handleLogin(const char *name,const char * pwd);  //处理来自客户端的登录请求
    void handleDownLine(const char *name); //处理客户端的下线请求
    QStringList handleAllOnlineUser();             //显示所有在线用户的请求
    int handleAddFriend(const char * his_Name,const char *my_Name);
    // bool handleReacher(const char *name);           //在数据库中查询该用户是否存在  上面是我的尝试，发现不行，使用int 类型，更加容易去获得该用户是否在线与否，是否存在与否
    int  handleReacherUsr(const char *name);
    QStringList handleFlushFriend(const char *name);
    bool handleDelete(const char * my_Name,const char *his_Name);//删除好友

private:
    QSqlDatabase m_db; //连接数据库

};

#endif // OPENDB_H
