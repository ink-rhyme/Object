#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include "amptyinfodialog.h"
#include <QMenu>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
public:
    void initWid();
    void connectSQL();
    void selectInfo();

signals:
    void readyread(QSqlDatabase&);

private slots:
    void on_btn_start_clicked();
    void on_btnRight();
    void getinfo();

private:
    AmptyInfoDialog *info;
    static QSqlDatabase db;
    QSqlQuery *quary;
    QMenu *m_btnRight_menu;
    QAction *m_btnRight_action;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
