
use webInfo

--创建信息表
create table infoTab
(
	文章标题 nvarchar(255) not null primary key,
	文章作者 nvarchar(50),
	发布时间 char(20),
	内容摘要 nvarchar(max),
	评论量   char(10),
	浏览量   char(10)
)

alter table infoTab
alter column 评论量 char(9)

insert into infoTab
	values('你是猪吗3','你才是猪','对','你是猪','00000','11111')

insert into infoTab values('机器学习实战之第一章 机器学习基础','nm-xy','2017-09-01 11:59','ApacheCN——专注于优秀开源项目维护的组织，不止于权威的文档视频技术支持 ...','1','291')

select * from infoTab

--alter table infoTab
--删除表表中数据
delete from infoTab

drop table infoTab

/*==============================================================*/

--创建文章内容表
create table detailInfo
(
	文章标题 nvarchar(255) not null primary key,
	文章内容 text
)
--插入数据
insert into detailInfo
	values('机器学习实战之第一章 机器学习基础','曾经有一份真挚的爱情放在我的面前我没有珍惜，知道后悔了才感觉追悔莫及')

select 文章内容 from detailInfo

--删除表表中数据
delete from detailInfo

select * from infoTab,detailInfo
	where infoTab.文章标题=detailInfo.文章标题

select * from detailInfo where 文章标题='你是猪吗3'

alter table detailInfo
add constraint PK primary key(文章标题)

alter table detailInfo
alter column 文章内容 nvarchar(max) not null

drop table detailInfo

delete from infoTab
delete from detailInfo

select * from infoTab
select * from detailInfo
