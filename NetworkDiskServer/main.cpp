#include "networkserver.h"

#include <QApplication>
#include"opendb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpenDB::getInstance().init();
    NetworkServer w;
    w.show();
    return a.exec();
}
