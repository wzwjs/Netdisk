#ifndef ONLINE_H
#define ONLINE_H

#include <QWidget>
#include "protocol.h"
#include <QListWidget>


namespace Ui {
class Online;
}

class Online : public QWidget
{
    Q_OBJECT

public:
    explicit Online(QWidget *parent = nullptr);
    ~Online();
    void showUsr(PDU *pdu);

private slots:
    void on_addFriend_pb_clicked();

private:
    Ui::Online *ui;
    QListWidget wiget;
};

#endif // ONLINE_H
