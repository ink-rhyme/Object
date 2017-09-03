#include "widget.h"
#include "ui_widget.h"
#include "httpnet.h"

#include <QtSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStringList>
#include <QMouseEvent>
#include <QPoint>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_btnRight_menu = NULL;


    ui->table_info->setContextMenuPolicy(Qt::CustomContextMenu);
    m_btnRight_menu = new QMenu(ui->table_info);
    m_btnRight_action = new QAction("文章内容",this);

    connect(m_btnRight_action,SIGNAL(triggered()),this,SLOT(getinfo()));
    connect(ui->table_info,SIGNAL(customContextMenuRequested(const QPoint&))
            ,this,SLOT(on_btnRight()));

    initWid();
}

Widget::~Widget()
{
    delete ui;
}

QSqlDatabase Widget::db;

void Widget::on_btnRight()
{
    if(ui->table_info->currentRow() < 0) return;
    m_btnRight_menu->addAction(m_btnRight_action);
    m_btnRight_menu->exec(QCursor::pos());
}

void Widget::initWid()
{
    ui->line_url->setText("http://www.cnblogs.com/sitehome/p/2");

    ui->table_info->setAlternatingRowColors(true);

    ui->table_info->setColumnCount(6);
    ui->table_info->setColumnWidth(0,150);
    ui->table_info->setColumnWidth(1,80);
    ui->table_info->setColumnWidth(2,80);
    ui->table_info->setColumnWidth(3,400);
    ui->table_info->setColumnWidth(4,100);
    ui->table_info->setColumnWidth(5,100);

    QStringList list;
    list<<"文章标题"<<"作者"<<"发布时间"<<"内容摘要"<<"评论量"<<"浏览量";
    ui->table_info->setHorizontalHeaderLabels(list);
    ui->table_info->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_info->verticalHeader()->setVisible(false);

    //设置整行选择
    ui->table_info->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置行高
    ui->table_info->verticalHeader()->setDefaultSectionSize(40);

    ui->table_info->setStyleSheet(
                "QTableView::item{alternate-background-color:rgb(100,100,100);}QTableView::item{alternate-background-color:rgb(0,255,0);}");
    ui->table_info->setAlternatingRowColors(true);
}

void Widget::selectInfo()
{
    ui->table_info->clearContents();
    ui->table_info->setRowCount(0);


    bool bRet = quary->exec(QString("select * from infoTab"));
    if(!bRet) return;
    QString info;
    int row = 0;
    while(quary->next()){
        ui->table_info->insertRow(row);

        info = quary->value("文章标题").toString();
        ui->table_info->setItem(row,0,new QTableWidgetItem(info));

        info = quary->value("文章作者").toString();
        ui->table_info->setItem(row,1,new QTableWidgetItem(info));

        info = quary->value("发布时间").toString();
        ui->table_info->setItem(row,2,new QTableWidgetItem(info));

        info = quary->value("内容摘要").toString();
        ui->table_info->setItem(row,3,new QTableWidgetItem(info));

        info = quary->value("评论量").toString();
        ui->table_info->setItem(row,4,new QTableWidgetItem(info));

        info = quary->value("浏览量").toString();
        ui->table_info->setItem(row,5,new QTableWidgetItem(info));


        info.clear();
        ++row;
    }
}

void Widget::getinfo()
{
    int row = ui->table_info->currentRow();
    if(row < 0) return;
    bool bRet = quary->exec(QString("select * from infoTab"));
    if(!bRet) return;
    while(quary->next()&&row--);
    QString title = quary->value("文章标题").toString();
    title.insert(0,'\'');
    title += '\'';
    quary->exec(QString("select * from detailInfo where 文章标题=%1").arg(title));
    {
        info = new AmptyInfoDialog;
        //写入内容
        quary->next();
        info->setTextTitle(quary->value("文章标题").toString());
        info->setTextBrowser(quary->value("文章内容").toString());
        info->exec();
        delete info;
        info = NULL;
    }
}

void Widget::connectSQL()
{
    //链接数据库
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                               "SERVER=%1;"
                               "DATABASE=%2;"
                               "UID=%3;"
                               "PWD=%4;")
                       .arg("INKRHYME\\SQLSERVER")
                       .arg("webInfo")
                       .arg("inkRhyme")
                       .arg("000000")
                       );
    if (!db.open())
    {

        QMessageBox::warning(0, qApp->tr("Cannot open database"),
                             db.lastError().databaseText(), QMessageBox::Cancel);
        return;
    }
    else
    {
        qDebug()<<"Connect to Database Success!";

        quary = new QSqlQuery(db);
    }
}

void Widget::on_btn_start_clicked()
{
    //链接数据库
    connectSQL();
    httpnet net(db,this);
    net.readHTML(QUrl(ui->line_url->text()));
    //查询信息
    selectInfo();
}
