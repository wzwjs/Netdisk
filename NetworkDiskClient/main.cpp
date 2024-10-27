#include "networkdisk.h"
#include <QApplication>
//#include "book.h"
#include "opewidget.h"

// #include "online.h"
// #include "friend.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Networkdisk w;
    // w.show();
    Networkdisk::getInstance().show();//通过单例模式去获得对象的引用，通过引用去获得show()函数

    // OpeWidget w;
    // w.show();
    //Book w;
   // w.show();

    //online 测试
    // Online w;
    // w.show();

    //fiened 好友测试
    // Friend w;
    // w.show();
    return a.exec();
}
