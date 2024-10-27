/********************************************************************************
** Form generated from reading UI file 'networkserver.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWORKSERVER_H
#define UI_NETWORKSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NetworkServer
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;

    void setupUi(QWidget *NetworkServer)
    {
        if (NetworkServer->objectName().isEmpty())
            NetworkServer->setObjectName(QString::fromUtf8("NetworkServer"));
        NetworkServer->resize(540, 540);
        verticalLayout = new QVBoxLayout(NetworkServer);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(NetworkServer);

        QMetaObject::connectSlotsByName(NetworkServer);
    } // setupUi

    void retranslateUi(QWidget *NetworkServer)
    {
        NetworkServer->setWindowTitle(QCoreApplication::translate("NetworkServer", "NetworkServer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NetworkServer: public Ui_NetworkServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWORKSERVER_H
