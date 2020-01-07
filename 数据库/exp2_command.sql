exp5:

create index supp_sname_desc_index on supp(sname desc);

create unique index project_jname_unique_index on project(jname);

create index spj_sno_asc_pno_desc_index on spj(sno asc, pno desc);

exec sp_helpindex supp;

exec sp_helpindex project;

exec sp_helpindex spj;

drop index supp_sname_desc_index on supp;

drop index project_jname_unique_index on project;

drop index spj_sno_asc_pno_desc_index on spj;

***************************************

exp6:
1.
(1)
select sno from spj where jno='j1';

(2)
select sno from spj where jno='j1' and pno = 'p1';

(3)
select sno from spj, part where spj.jno='j1' and spj.pno = part.pno and color='红';

(4)
select jno from spj where jno not in (select jno from spj, part, supp where supp.city = '天津' and color = '红' and supp.sno = spj.sno
and part.pno = spj.pno);

(5)
select pno from spj where sno = 's1';
select jno from spj where pno = 'p1' and jno not in (select pno from spj where pno = 'p2');

2.
(1)
select sname,city from supp;
(2)
select pname,color,weight from part;
(3)
select jno from spj where sno='s1';
(4)
select pname,qty from spj,part where part.pno=spj.pno and spj.jno='j2';
(5)
select pno from spj,supp where supp.sno=spj.sno and city='上海';
(6)
select jname from spj,supp,project where supp.sno=spj.sno and supp.city='上海' and project.jno=spj.jno;
(7)
注意: 
select jno from spj  where jno not in (select jno from spj,supp where supp.sno=spj.sno and supp.city='天津');
适用于jno是唯一或不唯一的情况.

注意: 
select jno from spj,supp where supp.sno=spj.sno and supp.city<>'天津';
适用于jno是唯一的情况

exp7:
(1)
select count(sname) as supp_count from supp;

(2)
select * from project where city = '北京';
select jname, count(city) as project_in_beijing from project where city = '北京' group by jname;

(3)
select * from (select sno, count(qty) as sno_qty from spj group by sno) as ha order by ha.sno_qty desc;

(4)
select * from (select jno, count(sno) as cnt from spj group by jno) as ha where ha.cnt > 3;

exp8:
create view exp8_test as select sno, spj.pno,qty from spj,project where spj.jno=project.jno and project.jname='三建';

select pno, qty from exp8_test;

select * from exp8_test where sno='s1';

exp9:
(1)
create trigger P_Ins on part instead of insert
as
begin
	declare @WEIGHT int
	select @WEIGHT = (select weight from part)
if(@WEIGHT < 0)
	print('Error, the inserted weight is slower than zero');
	rollback transaction
end
;











