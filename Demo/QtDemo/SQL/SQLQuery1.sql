
use webInfo

--������Ϣ��
create table infoTab
(
	���±��� nvarchar(255) not null primary key,
	�������� nvarchar(50),
	����ʱ�� char(20),
	����ժҪ nvarchar(max),
	������   char(10),
	�����   char(10)
)

alter table infoTab
alter column ������ char(9)

insert into infoTab
	values('��������3','�������','��','������','00000','11111')

insert into infoTab values('����ѧϰʵս֮��һ�� ����ѧϰ����','nm-xy','2017-09-01 11:59','ApacheCN����רע�����㿪Դ��Ŀά������֯����ֹ��Ȩ�����ĵ���Ƶ����֧�� ...','1','291')

select * from infoTab

--alter table infoTab
--ɾ�����������
delete from infoTab

drop table infoTab

/*==============================================================*/

--�����������ݱ�
create table detailInfo
(
	���±��� nvarchar(255) not null primary key,
	�������� text
)
--��������
insert into detailInfo
	values('����ѧϰʵս֮��һ�� ����ѧϰ����','������һ����ֿ�İ�������ҵ���ǰ��û����ϧ��֪������˲Ÿо�׷��Ī��')

select �������� from detailInfo

--ɾ�����������
delete from detailInfo

select * from infoTab,detailInfo
	where infoTab.���±���=detailInfo.���±���

select * from detailInfo where ���±���='��������3'

alter table detailInfo
add constraint PK primary key(���±���)

alter table detailInfo
alter column �������� nvarchar(max) not null

drop table detailInfo

delete from infoTab
delete from detailInfo

select * from infoTab
select * from detailInfo
