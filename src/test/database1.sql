CREATE TABLE student (
  no INT NOT NULL,
  name VARCHAR(255) NOT NULL,
  gender ENUM('female', 'male') NOT NULL,
  age INT NOT NULL,
  PRIMARY KEY (no)
);

desc student;

CREATE TABLE dept (
  no INT NOT NULL,
  name VARCHAR(255) NOT NULL,
  PRIMARY KEY (no)
);

desc dept;

CREATE TABLE course (
  no INT NOT NULL,
  name VARCHAR(255) NOT NULL,
  credit INT NOT NULL,
  PRIMARY KEY (no)
);

desc course;

CREATE TABLE score (
  s_no INT NOT NULL,
  c_no INT NOT NULL,
  score INT NOT NULL,
  PRIMARY KEY (s_no, c_no),
  FOREIGN KEY (s_no) REFERENCES student (no),
  FOREIGN KEY (c_no) REFERENCES course (no)
);

desc course;

SELECT table_name, constraint_name, constraint_type
FROM information_schema.TABLE_CONSTRAINTS
WHERE table_name = 'student';

SELECT table_name, constraint_name, constraint_type
FROM information_schema.TABLE_CONSTRAINTS
WHERE table_name = 'dept';

SELECT table_name, constraint_name, constraint_type
FROM information_schema.TABLE_CONSTRAINTS
WHERE table_name = 'course';

SELECT table_name, constraint_name, constraint_type
FROM information_schema.TABLE_CONSTRAINTS
WHERE table_name = 'score';

ALTER TABLE student
MODIFY COLUMN name CHAR(15) NOT NULL;
desc student;

ALTER TABLE student
ADD d_no INT NOT NULL,
ADD FOREIGN KEY (d_no) REFERENCES dept(no);

desc student;

SELECT table_name, constraint_name, constraint_type
FROM information_schema.TABLE_CONSTRAINTS
WHERE table_name = 'student';

ALTER TABLE course
ADD d_no INT NOT NULL,
ADD FOREIGN KEY (d_no) REFERENCES dept(no);

desc course;

SELECT table_name, constraint_name, constraint_type
FROM information_schema.TABLE_CONSTRAINTS
WHERE table_name = 'course';


ALTER TABLE score
MODIFY s_no INT,
MODIFY c_no INT,
MODIFY score INT;

ALTER TABLE score
DROP FOREIGN KEY score_ibfk_1, 
DROP FOREIGN KEY score_ibfk_2;

desc score;

SELECT table_name, constraint_name, constraint_type
FROM information_schema.TABLE_CONSTRAINTS
WHERE table_name = 'score';


ALTER TABLE score
MODIFY s_no INT NOT NULL,
MODIFY c_no INT NOT NULL,
MODIFY score INT NOT NULL;

ALTER TABLE score
ADD FOREIGN KEY (s_no) REFERENCES student(no),
ADD FOREIGN KEY (c_no) REFERENCES course(no);

desc score;

SELECT table_name, constraint_name, constraint_type
FROM information_schema.TABLE_CONSTRAINTS
WHERE table_name = 'score';