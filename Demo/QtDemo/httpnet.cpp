#include "httpnet.h"

#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QByteArray>
#include <QtCore>

#include <algorithm>

httpnet::httpnet(QSqlDatabase db,QObject *parent) : QObject(parent)
{
    m_query = new QSqlQuery(db);
    connect(this,SIGNAL(readyReadContent()),this,SLOT(readContentHTML()));

}

httpnet::~httpnet()
{
    delete m_query;
}

//抓取网页
void httpnet::readHTML(QUrl url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(url));

    QEventLoop loop;
    connect(manager,SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
    loop.exec();

    QByteArray arry;
    arry = reply->readAll();
    html = QString(arry);

    read();
}

//抓取网页文章信息
void httpnet::read()
{
    QRegExp regBody("<div\\sclass=\"post_item_body\">([\\s\\S].*)</div>");
    regBody.setMinimal(true);


    //a标签 文章标题  作者名字 评论 阅读
    QRegExp regA("<a[^>]*>(.*)</a>");
    regA.setMinimal(true);

    //a标签 文章内容(超链接)
    QRegExp regH("class=\"titlelnk\"\\shref=\"([\\S^\"].*)\"\\starget");
    regH.setMinimal(true);

    //p标签 文章内容
    QRegExp regP("<p\\sclass=\"post_item_summary\">(.*)</p>");
    regP.setMinimal(true);

    //提取时间(注意要用括号括起来，代表要提取的内容)
    QRegExp regTime("(\\d{4}-\\d{2}-\\d{2}\\s\\d{2}:\\d{2})");
    regTime.setMinimal(true);

    //提取评论 阅读次数如：评论（10）-》10
    QRegExp regNumbernew("(\\d+)");

    //文章浏览量(注意屏蔽掉regNumbernew.setMinimal(true)，不然数字只有1位)
    //regNumbernew.setMinimal(true);

    int pos=0;
    QString content;

    while((pos = regBody.indexIn(html,pos)) != -1){
        regH.indexIn(regBody.cap(1),0);
        urlList<<regH.cap(1);

        //更新匹配位置
        pos += regBody.matchedLength();
    }
    emit readyReadContent();

    pos = 0;
    content.clear();
    //每次循环获取一篇博客
    while ((pos = regBody.indexIn(html,pos)) != -1)
    {
        content = regBody.cap(1).trimmed();

        //截取每篇博客的信息，strList中依次包括（标题、头像、作者、评论、阅读量）
        int pos1 = 0;
        QStringList strList;
        while ((pos1 = regA.indexIn(content,pos1)) != -1)
        {
            strList<<regA.cap(1).trimmed();
            pos1 += regA.matchedLength();
        }

        //文章标题
        tabInfo  objModel;

        //如果文章简介中没有缩略图
        if(strList.count() == 4)
        {
            objModel._articleTitle =strList[0];

            //文章作者
            objModel._articleAuthor = strList[1];

            //文章评论量
            regNumbernew.indexIn(strList[2],0);

            objModel._articleComment = regNumbernew.cap(1).toInt();

            //文章浏览量
            regNumbernew.indexIn(strList[3],0);
            objModel._articleView = regNumbernew.cap(1).toInt();

        }
        else
        {//如果文章简介中有缩略图
            objModel._articleTitle =strList[0];

            //文章作者
            objModel._articleAuthor = strList[2];

            //文章评论量
            regNumbernew.indexIn(strList[3],0);
            objModel._articleComment = regNumbernew.cap(1).toInt();

            //文章浏览量(注意屏蔽掉regNumbernew.setMinimal(true)，不然数字只有1位)
            regNumbernew.indexIn(strList[4],0);
            objModel._articleView = regNumbernew.cap(1).toInt();
        }

        //文章内容摘要
        regP.indexIn(content,0);
        objModel._articleSummary = regP.cap(1).trimmed();
        if(objModel._articleSummary[0] == '\t')
            objModel._articleSummary = objModel._articleSummary.right(objModel._articleSummary.size() - 1);

        //选取超链接,并替换
        QRegExp regP1("(<a[^>]+>.*</a>)");
        QString strNew = objModel._articleSummary.replace(regP1, " ");
        objModel._articleSummary = strNew;

        //文章发布时间
        regTime.indexIn(content,0);
        objModel._articleTime = regTime.cap(1);

        insertDatabasetab1(objModel);

        //更新匹配位置
        pos += regBody.matchedLength();
    }
}

//解析文章内容字符串
void httpnet::readContent(QString strInfo)
{
    detailInfo data;

    //data中写入内容
    //<div.....</div>间内容
    QRegExp regBody("<div\\sid=\"post_detail\">([^>][\\s\\S].*)<div\\sid=\"MySignature\">");
    regBody.indexIn(strInfo,0);
    QRegExp regH("<a\\sid=\"cb_post_title_url\"([^>][\\s\\S].*)>([\\s\\S].*)</a>");
    regH.setMinimal(true);
    regH.indexIn(regBody.cap(1),0);
    data._articleTitle = regH.cap(2);

    //文章内容html
    QRegExp regBHtml("<div\\sid=\"cnblogs_post_body\"([^>]*)>([\\s\\S].*)<div\\sid=\"MySignature\">");
    regBHtml.setMinimal(true);
    regBHtml.indexIn(regBody.cap(0),0);
    data._articleContent = regBHtml.cap(2);

    QRegExp regB("<[^>]+>|&nbsp;|&lt;|&gt;|&mdash;");
    data._articleContent = data._articleContent.replace(regB,"");
    insertDatabasetab2(data);
}

//抓取文章内容信息
void httpnet::readContentHTML()
{
    for(const auto& a:urlList) {

        QNetworkAccessManager* manager = new QNetworkAccessManager;
        QUrl url(a);
        QNetworkReply* reply = manager->get(QNetworkRequest(url));

        QEventLoop loop;
        connect(manager,SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
        loop.exec();

        QByteArray info = reply->readAll();
        readContent(QString(info));
    }
}

void httpnet::insertDatabasetab1(tabInfo &data)
{
    m_query->prepare(QString("insert into infoTab values('%1','%2','%3','%4','%5','%6')")
                  .arg(data._articleTitle)
                  .arg(data._articleAuthor)
                  .arg(data._articleTime)
                  .arg(data._articleSummary)
                  .arg(QString::number(data._articleComment))
                  .arg(QString::number(data._articleView))
            );
    m_query->exec();
}

void httpnet::insertDatabasetab2(detailInfo &data)
{
    QString& str = data._articleContent;
    QString::iterator iter = str.begin();
    while(iter != str.end()){
        if(*iter == '\'')
            *iter = 97;
        ++iter;
    }
    m_query->prepare(QString("insert into detailInfo values('%1','%2')")
                .arg(data._articleTitle)
                .arg(data._articleContent)
            );
    if(!m_query->exec())
        qDebug()<<data._articleContent;
}
