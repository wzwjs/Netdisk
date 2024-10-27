#include "opewidget.h"

OpeWidget::OpeWidget(QWidget *parent)
    : QWidget{parent}
{
    //界面的左边窗口
    m_pListW=new QListWidget(this);
    m_pListW->addItem("好友");
    m_pListW->addItem("图书");


    m_pFriend=new Friend;
    m_pBook=new Book;

    //因为这两个窗口每一次只能显示一个，通过堆上窗口来显示

    m_pSW=new QStackedWidget;  //默认显示第一个窗口
    m_pSW->addWidget(m_pFriend);
    m_pSW->addWidget(m_pBook);

    QHBoxLayout *pMain=new QHBoxLayout;
    pMain->addWidget(m_pListW);
    pMain->addWidget(m_pSW);
    setLayout(pMain);

    connect(m_pListW,SIGNAL(currentRowChanged(int)),m_pSW,SLOT(setCurrentIndex(int)));//通过信号槽的关联，使得界面呈现点击界面转化的功能
    //
}

OpeWidget &OpeWidget::getInstance()
{
    static OpeWidget instance;//做一个单例操作，产生一个操作界面对象的静态对象
    return instance;          //在调用这个函数的时候，始终返回的是同一个对象的引用
}

Friend *OpeWidget::getfriend()
{
    return m_pFriend;
}

Book *OpeWidget::getBook()
{
    return m_pBook;
}
