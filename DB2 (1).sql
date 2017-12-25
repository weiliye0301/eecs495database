DELIMITER //
drop  trigger if  exists low_enrollment;
//
DELIMITER ;
delimiter //
create trigger low_enrollment after update on uosoffering 
for each row
begin
	set @warning=0;
  if new.Enrollment<(0.5*new.MaxEnrollment) then
    set @warning=1;
    signal sqlstate '01000' set message_text="Warning: Enrollment goes below 50% of capacity!";
  end if;
end //
delimiter ;

DELIMITER //
drop  procedure if  exists withdraw;
//
DELIMITER ;

DELIMITER //

CREATE PROCEDURE withdraw(a char(8), b int(11)) 
 


BEGIN
declare c int DEFAULT month (CURDATE());

set @q= 0 ;


if (a not in (select UoSCode FROM transcript WHERE StudId = b and Year = YEAR (CURDATE()) and 
(
( c <= 11 and c>=9 and Semester = 'Q1') OR
( (c <= 2 or c>=12) and Semester = 'Q2') OR
( c <= 5 and c>=3 and Semester = 'Q3') OR
( c <= 8 and c>=6 and Semester = 'Q4') 
))) then
set @q = 1;
select @q;
else
if (a not in (select UoSCode FROM transcript WHERE Grade is null and StudId = b and Year = YEAR (CURDATE()) and 
(
( c <= 11 and c>=9 and Semester = 'Q1') OR
( (c <= 2 or c>=12) and Semester = 'Q2') OR
( c <= 5 and c>=3 and Semester = 'Q3') OR
( c <= 8 and c>=6 and Semester = 'Q4') 
)))then

set @q = 2;
select @q;
else
DELETE FROM transcript WHERE UoSCode = a and Grade is null and StudId = b and Year = YEAR (CURDATE()) and 
(
( c <= 11 and c>=9 and Semester = 'Q1') OR
( (c <= 2 or c>=12) and Semester = 'Q2') OR
( c <= 5 and c>=3 and Semester = 'Q3') OR
( c <= 8 and c>=6 and Semester = 'Q4') 
)
; 
UPDATE uosoffering SET Enrollment = Enrollment - 1 WHERE UoSCode = a and Year = YEAR (CURDATE())and 
(
( c <= 11 and c>=9 and Semester = 'Q1') OR
( (c <= 2 or c>=12) and Semester = 'Q2') OR
( c <= 5 and c>=3 and Semester = 'Q3') OR
( c <= 8 and c>=6 and Semester = 'Q4') 
); 
SELECT @q, @warning, UoSCode, Grade, Year, Semester FROM transcript WHERE StudId = b and Year = YEAR (CURDATE()) and 
(
( c <= 11 and c>=9 and Semester = 'Q1') OR
( (c <= 2 or c>=12) and Semester = 'Q2') OR
( c <= 5 and c>=3 and Semester = 'Q3') OR
( c <= 8 and c>=6 and Semester = 'Q4') 
)
; 
end if;
end if;


END 
//
DELIMITER ;

DELIMITER //
drop  procedure if  exists addclass;
//
DELIMITER ;

DELIMITER //

CREATE PROCEDURE addclass
( IN studentID int,
  IN coursename char(8),
  IN courseyear int,
  IN coursesemester char(2))

BEGIN
set @q = 0;
#START TRANSACTION;
if(coursename not in (SELECT distinct a.UoSCode FROM uosoffering a where a.Year = courseyear and a.Semester = coursesemester))
then
set@q=1;
select @q;
else 
if(coursename not in (SELECT distinct a.UoSCode 
FROM uosoffering a, transcript b, requires c 
where a.Year =  courseyear
	and a.Semester = coursesemester 
	and ( (a.UoSCode = c.UoSCode and c.PrereqUoSCode = b.UoSCode and b.StudId = studentID and ( b.Grade = 'P' or b.Grade = 'CR' or b.Grade = 'D') and b.Year <= courseyear)
	or (a.UoSCode  not in (select UoSCode From requires)))))
then
set@q=2;
select distinct @q, a.PrereqUoSCode from requires a where a.UoSCode = coursename;
else 
if (coursename not in (SELECT distinct a.UoSCode 
FROM uosoffering a, transcript b, requires c 
where a.Year =  courseyear
	and a.Semester = coursesemester 
	and a.Enrollment <=a.MaxEnrollment 
	and ( (a.UoSCode = c.UoSCode and c.PrereqUoSCode = b.UoSCode and b.StudId = studentID and ( b.Grade = 'P' or b.Grade = 'CR' or b.Grade = 'D') and b.Year <=courseyear)
	or (a.UoSCode  not in (select UoSCode From requires))))
) then

set@q=3;
select @q;
else
if (coursename in (select UoSCode from transcript a where a.StudId = studentID and a.Year = courseyear and a.Semester = coursesemester and a.UoSCode =  coursename))
then
set@q=4;
select @q;
else
Insert INTO transcript
values(studentID,coursename,coursesemester,courseyear,null);
update uosoffering
set uosoffering.Enrollment=uosoffering.Enrollment+1
where uosoffering.Uoscode=coursename and uosoffering.Semester=coursesemester and uosoffering.Year=courseyear;
select @q;
end if;
end if;
end if;
end if;

COMMIT;
END//	

DELIMITER ;

