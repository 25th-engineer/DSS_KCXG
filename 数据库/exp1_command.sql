create database SPJDB
on
(name=SPJDB_data,
filename='D:\xmgl\SPJDB.mdf' ,
size=10,
maxsize=60,
filegrowth=5%)
log on
( name=SPJDB_log, 
filename='D:\xmgl\SPJDB_log.ldf ' , 
size=4, 
maxsize=10, 
filegrowth=1);

use SPJDB;

create table Project(
JNO varchar(5) primary key,
JNAME varchar(20) not null,
CITY varchar(20) not null
);

create table Part(
PNO varchar(5) primary key,
PNAME varchar(20) not null,
COLOR varchar(10) not null,
WEIGHT integer not null
);

create table Supp(
SNO varchar(5) primary key,
SNAME varchar(10) not null,
STATUS integer not null,
CITY varchar(20) not null
);

create table SPJ(
SNO varchar(5) not null,
PNO varchar(5) not null,
JNO varchar(5) not null,
QTY integer not null
);

create table Supp1(
SNO varchar(5) primary key,
SNAME varchar(10) not null,
STATUS integer not null,
CITY varchar(20) not null
);

alter table Supp1 add phone varchar(11);

drop table Supp1;

Bulk  insert  Project
From  'E:\sql_data\J_table.csv'
With
(
fieldterminator=',',
rowterminator='\n'
);

Bulk  insert  Part
From  'E:\sql_data\P_table.csv'
With
(
fieldterminator=',',
rowterminator='\n'
);

Bulk  insert  Supp
From  'E:\sql_data\S_table.csv'
With
(
fieldterminator=',',
rowterminator='\n'
);

Bulk  insert  SPJ
From  'E:\sql_data\SPJ_table_1.csv'
With
(
fieldterminator=',',
rowterminator='\n'
);

alter table SPJ add constraint id primary key(SNO, PNO, JNO, QTY);
alter table SPJ add id int identity(1,1) not null;

Bulk  insert  SPJ
From  'E:\sql_data\SPJ_table.csv'
With
(
fieldterminator=',',
rowterminator='\n'
);


insert into Project values('J62','问题','乌鲁木齐');
insert into Project values('J63','号角','临沂');
insert into Project values('J64','老兵','唐山');
insert into Project values('J65','万岁','漳州');
insert into Project values('J66','我爱','赣州');
insert into Project values('J67','学习','凯里');
insert into Project values('J68','逝景','宣城');

delete from Project where JNO = 'J62';
delete from Project where JNO = 'J63';
delete from Project where JNO = 'J64';
delete from Project where JNO = 'J65';
delete from Project where JNO = 'J66';
delete from Project where JNO = 'J67';
delete from Project where JNO = 'J68';

update Project set JNAME='大宝贝' where JNO = 'J55';
update Project set JNAME='乌黑拉黑' where JNO = 'J56';
update Project set JNAME='巴啦啦' where JNO = 'J57';
update Project set JNAME='小魔仙' where JNO = 'J58';
update Project set JNAME='全身变' where JNO = 'J59';

insert into Part values('P57','积草屯粮','黄',97);
insert into Part values('P58','守阍','绿',17);
insert into Part values('P59','追风捕影','蓝',27);
insert into Part values('P60','月品','靛',47);
insert into Part values('P61','讯号','紫',49);
insert into Part values('P62','醒聩震聋','红',39);
insert into Part values('P63','故公','绿',91);
insert into Part values('P64','高良姜','蓝',24);
insert into Part values('P65','高谈弘论','红',15);
insert into Part values('P66','自识','蓝',52);
insert into Part values('P67','魁俊','白',45);

     
update Part set PNAME='叨在知己' where PNO = 'P40';
update Part set PNAME='戴渊剑' where PNO = 'P41';
update Part set PNAME='鸿序' where PNO = 'P42';
update Part set PNAME='媒互人' where PNO = 'P43';
update Part set PNAME='火烧' where PNO = 'P44';
update Part set PNAME='袭冒' where PNO = 'P45';

delete from Part where PNO = 'P65';
delete from Part where PNO = 'P66';
delete from Part where PNO = 'P67';

insert into Supp values('S95','修脡',58,'福州');
insert into Supp values('S96','簸土扬沙',62,'厦门');
insert into Supp values('S97','中捐',68,'成都');
insert into Supp values('S98','根体',99,'杭州');
insert into Supp values('S99','拷纱',77,'西安');
insert into Supp values('S100','提箱',57,'苏州');
insert into Supp values('S101','厘降',68,'南京');
insert into Supp values('S102','皂罗',73,'长沙');
insert into Supp values('S103','周谒',1,'郑州');
insert into Supp values('S105','缫席',86,'青岛');

   
update Supp set SNAME='训习' where SNO = 'S66';
update Supp set SNAME='豫闻' where SNO = 'S67';
update Supp set SNAME='胡土克图' where SNO = 'S68';
update Supp set SNAME='鸿鸟' where SNO = 'S69';

delete from Supp where SNO = 'S67';
delete from Supp where SNO = 'S68';
delete from Supp where SNO = 'S69';


insert into SPJ values('S9','P13','J42',140);
insert into SPJ values('S9','P1','J38',298);
insert into SPJ values('S86','P31','J61',391);
insert into SPJ values('S77','P1','J67',646);
insert into SPJ values('S73','P32','J64',282);
insert into SPJ values('S71','P30','J2',111);
insert into SPJ values('S97','P16','J35',228);

update SPJ set SNO='S1' where ID = 50;
update SPJ set SNO='S4' where ID = 51;
update SPJ set SNO='S8' where ID = 52;
update SPJ set SNO='S16' where ID = 53;

delete from SPJ where ID = 50;
delete from SPJ where ID = 51;
delete from SPJ where ID = 52;

bcp spjdb.dbo.project out  E:\sql_data\project.sql -f sql -T -c

bcp spjdb.dbo.part out  E:\sql_data\part.sql -f sql -T -c

bcp spjdb.dbo.supp out  E:\sql_data\supp.sql -f sql -T -c

bcp spjdb.dbo.spj out  E:\sql_data\spj.sql -f sql -T -c