#ifndef OPEWIDGET_H
#define OPEWIDGET_H

#include <QWidget>
#include <QListWidget>  //左边的列表

#include "book.h"
#include "friend.h"
#include <QStackedWidget>  //堆上窗口，可以包含很多窗口，但每次执行一个


class OpeWidget : public QWidget
{
    Q_OBJECT      //正因为这个槽，qt程序才能使用信号槽
public:
    explicit OpeWidget(QWidget *parent = nullptr);
    //将打开操作封装成为一个单例模式
    static OpeWidget &getInstance();//这个是什么东西?
    Friend *getfriend();
    Book *getBook();

private:
    QListWidget *m_pListW;   //创建一个list表去存储widget
    Friend *m_pFriend;       //初始化好友窗口
    Book *m_pBook;             //初始化book窗口
    QStackedWidget  *m_pSW;     //初始化堆上widget窗口保存
signals:
};

#endif // OPEWIDGET_H
