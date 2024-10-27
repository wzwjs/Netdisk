#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include <QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QListWidget>
#include "protocol.h"


class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = nullptr);
    void updateFileList( PDU *pdu);
    QString enterDir();
    void clearEnterDir();

public slots:
    //创建文件夹
    void create_files();
    //刷新文件
    void flushFile();
    //删除文件夹
    void delete_Dir();
    //重命名文件夹
    void rename_Files();
    void enter_Dir(const QModelIndex &index);

private:
    QPushButton *m_reback;
    QPushButton *m_create_files;    //文件夹
    QPushButton *m_delete_files;
    QPushButton *m_reName_files;
    QPushButton *m_flush_files;
    QPushButton *m_up_file;         //文件
    QPushButton *m_down_file;
    QPushButton *m_delete_file;
    QPushButton *m_share_file;

    QListWidget *m_pFileListWidget; //文件列表


    QListWidget *m_pBookListWidget;

    QString m_strEnterDir;      //进入的目录

signals:
};

#endif // BOOK_H
