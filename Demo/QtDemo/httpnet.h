#ifndef HTTPNET_H
#define HTTPNET_H

#include "table.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>

class httpnet : public QObject
{
    Q_OBJECT
public:
    explicit httpnet(QSqlDatabase db,QObject *parent = 0);
    ~httpnet();

signals:
    void readyRead();
    void readyReadContent();
public slots:
    void readContentHTML();

public:
    void readHTML(QUrl);
    void read();                            //读取主网页的文章信息
    void readContent(QString);              //读取文章内容
    void insertDatabasetab1(tabInfo&);      //文章信息表插入
    void insertDatabasetab2(detailInfo&);   //文章内容表插入

private:
    QSqlQuery*      m_query;    //数据库操作

    QString         html;       //网页内容
    QList<QString>  urlList;    //存储文章url
};

#endif // HTTPNET_H
