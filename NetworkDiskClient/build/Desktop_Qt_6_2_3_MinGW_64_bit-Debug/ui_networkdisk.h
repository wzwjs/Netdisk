/********************************************************************************
** Form generated from reading UI file 'networkdisk.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWORKDISK_H
#define UI_NETWORKDISK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Networkdisk
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *name_lan;
    QLineEdit *name_le;
    QHBoxLayout *horizontalLayout_3;
    QLabel *pwd_lan;
    QLineEdit *pwd_le;
    QHBoxLayout *horizontalLayout;
    QPushButton *cancel_pb;
    QPushButton *regist_pb;
    QPushButton *login_pb;

    void setupUi(QWidget *Networkdisk)
    {
        if (Networkdisk->objectName().isEmpty())
            Networkdisk->setObjectName(QString::fromUtf8("Networkdisk"));
        Networkdisk->resize(566, 442);
        widget = new QWidget(Networkdisk);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(50, 60, 461, 311));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        name_lan = new QLabel(widget);
        name_lan->setObjectName(QString::fromUtf8("name_lan"));
        QFont font;
        font.setPointSize(18);
        name_lan->setFont(font);
        name_lan->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(name_lan);

        name_le = new QLineEdit(widget);
        name_le->setObjectName(QString::fromUtf8("name_le"));

        horizontalLayout_2->addWidget(name_le);

        horizontalLayout_2->setStretch(0, 2);
        horizontalLayout_2->setStretch(1, 5);

        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pwd_lan = new QLabel(widget);
        pwd_lan->setObjectName(QString::fromUtf8("pwd_lan"));
        pwd_lan->setFont(font);
        pwd_lan->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(pwd_lan);

        pwd_le = new QLineEdit(widget);
        pwd_le->setObjectName(QString::fromUtf8("pwd_le"));
        pwd_le->setEchoMode(QLineEdit::Password);

        horizontalLayout_3->addWidget(pwd_le);

        horizontalLayout_3->setStretch(0, 2);
        horizontalLayout_3->setStretch(1, 5);

        gridLayout->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cancel_pb = new QPushButton(widget);
        cancel_pb->setObjectName(QString::fromUtf8("cancel_pb"));
        QFont font1;
        font1.setPointSize(16);
        cancel_pb->setFont(font1);

        horizontalLayout->addWidget(cancel_pb);

        regist_pb = new QPushButton(widget);
        regist_pb->setObjectName(QString::fromUtf8("regist_pb"));
        regist_pb->setFont(font1);

        horizontalLayout->addWidget(regist_pb);

        login_pb = new QPushButton(widget);
        login_pb->setObjectName(QString::fromUtf8("login_pb"));
        login_pb->setFont(font1);

        horizontalLayout->addWidget(login_pb);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(Networkdisk);

        QMetaObject::connectSlotsByName(Networkdisk);
    } // setupUi

    void retranslateUi(QWidget *Networkdisk)
    {
        Networkdisk->setWindowTitle(QCoreApplication::translate("Networkdisk", "\347\275\221\347\233\230\345\256\242\346\210\267\347\253\257", nullptr));
        name_lan->setText(QCoreApplication::translate("Networkdisk", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        pwd_lan->setText(QCoreApplication::translate("Networkdisk", "\345\257\206\347\240\201\357\274\232", nullptr));
        pwd_le->setText(QString());
        cancel_pb->setText(QCoreApplication::translate("Networkdisk", "\346\263\250\351\224\200", nullptr));
        regist_pb->setText(QCoreApplication::translate("Networkdisk", "\346\263\250\345\206\214", nullptr));
        login_pb->setText(QCoreApplication::translate("Networkdisk", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Networkdisk: public Ui_Networkdisk {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWORKDISK_H
