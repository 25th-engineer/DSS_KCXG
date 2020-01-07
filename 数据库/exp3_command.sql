create trigger p_ins
on part
for insert, update
as
if (select weight from inserted) <= 0
begin
    raiserror('the value cannot be inserted into table part since it is lower than', 16, 1)
    rollback
end

create trigger p_ins
	on part for insert, update
	as
	declare @tmp_pno varchar(6), @tmp_pname varchar(16)
	declare @tmp_color varchar(8), @tmp_weight int
	select @tmp_pno = pno, @tmp_pname = pname, @tmp_color = color, @tmp_weight = weight from inserted
	if( @tmp_weight < 0 )
	begin
		raiserror('the value cannot be inserted into table part since it is lower than', 16, 1)
		rollback
	end

create trigger J_Update
	on project for update
	as
	declare @tmp_jno varchar(6)
	declare @tmp_jname1 varchar(16), @tmp_city1 varchar(8)
	declare @tmp_jname2 varchar(16), @tmp_city2 varchar(8)
	select @tmp_jname1 = jname, @tmp_city1 = city from inserted
	select @tmp_jname2 = jname, @tmp_city2 = city from deleted
	if( @tmp_jname1 <> @tmp_jname2 and @tmp_city1 <> @tmp_city2 )
	begin
		raiserror( 'it is forbidden to change the project name and its located city simultaneously', 16, 1 )
		rollback transaction
	end

update Project set jname = '无理超人', city = '合肥' where id = 52;

create trigger t_dele_Supp
	on Supp for delete
	as
	declare @tmp_sno1 varchar(6), @tmp_sname1 varchar(16), @tmp_status1 int, @tmp_city1 varchar(8)
	--declare @tmp_sno2 varchar(6), @tmp_sname2 varchar(16), @tmp_status2 int, @tmp_city2 varchar(8)
	--select @tmp_sno2 from spj where @tmp_sname2 = spj.sno
	select @tmp_sno1 = sno, @tmp_sname1 = sname, @tmp_status1 = status, @tmp_city1 = city from deleted
	--select @tmp_sno2 = sno, @tmp_sname2 = sname, @tmp_status2 = status, @tmp_city2 = city from deleted
	--if( @tmp_sno1 = @tmp_sno2 )
	begin
		delete from spj where sno = @tmp_sno1
		print('all the relevant data involed in the deleted data in SPJ has been removed')
	end

select spj.JNO 工程号, jname 工程名, spj.pno 零件号, pname 零件名, sum(qty) 总数量
	into j_p_qty
	from part, project, spj
	where part.pno = spj.pno and project.jno = spj.jno
	group by spj.jno, jname, spj.pno, pname
	order by 工程号;

--该命令有问题
create trigger ins_TO_spj
on spj for insert
as
	declare @tmp_jno varchar(6), @tmp_pno varchar(6), @tmp_qty smallint
	select @tmp_jno = jno, @tmp_pno = pno from inserted
	if not exists (select * from j_p_qty where '工程号' = @tmp_jno and '零件号' = @tmp_pno)
		begin
			raiserror('the data is not allowed to inserted into the table spj since the PNO of tuple does not exist in the table j_o_qty ', 16, 1)
			rollback transaction
		end
	else
		begin
			select @tmp_qty = qty from inserted
			update j_p_qty set 总数量 = 总数量 + @tmp_qty where '工程号' = @tmp_jno and '零件号' = @tmp_pno
		end

insert into spj values('S2', 'P456', 'J3', 555);
