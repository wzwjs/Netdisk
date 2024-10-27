#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include <QTextEdit>  //信息的显示
#include<QListWidget> //存放增加的好友
#include<QLineEdit>   //存放信息的输入
#include<QPushButton>  //按钮
#include <QVBoxLayout> //垂直布局
#include<QHBoxLayout>   //水平布局
#include<QLabel>       //显示当前登录用户名

#include "online.h"   //显示在线


class Friend : public QWidget
{
    Q_OBJECT
public:
    static Friend &getinstance();
    explicit Friend(QWidget *parent = nullptr);
    void showAllOnLineUsr(PDU *pdu);//接收数据显示用户
    //保存查找用户接收的人名
    QString m_strSearchName;
    void updataFriendList(PDU *pdu);
public slots:
    void showOnline();//发出请求显示在线用户
    //查找用户
    void searchUsr();
    //刷新好友列表的槽函数
    void flushFriend();
    //删除用户
    void delFriend();
    //私聊的槽函数
    void privateChat();
    //群聊
    void groupChat();
    //显示群聊消息
    void show_group_msg(PDU *pdu);
signals:

    //直接写的布局，就直接将对应控件的头文件包含，并对对应的头文件初始化，
private:
    QTextEdit * m_pShowMsgTE;  //显示信息的
    QListWidget *m_pFriendListWidget;//显示好友列表
    QLineEdit *m_pInputMsgLE;        //显示信息输入框

    QPushButton *m_pDelFriendDB;
    QPushButton *m_FlushFriendpB;
    QPushButton *m_pShowOnlineUsrPB;  //显示在线
    QPushButton *m_pSearchUsrPB;
    QPushButton *m_pMsgSendDB;       //发信息
    QPushButton *m_poPrivateChatPB; //私聊

    //实现点击显示在线，就让弹出一个online窗口
    Online *m_pOnline;    //将online界面跟friend界面关联起来

    QLabel *m_current_name;      //显示当前用户名


};

#endif // FRIEND_H
