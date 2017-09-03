#ifndef TABLE_H
#define TABLE_H

#include <QString>

typedef struct Info{
    QString _articleTitle;      //文章标题
    QString _articleAuthor;     //文章作者
    QString _articleSummary;    //文章摘要
    QString _articleTime;       //发布时间
    int     _articleComment;    //评论量
    int     _articleView;       //浏览量
}tabInfo;

typedef struct detailInfo{
    QString _articleTitle;      //文章标题
    QString _articleContent;    //文章内容
}tabDetail;

#endif // TABLE_H
