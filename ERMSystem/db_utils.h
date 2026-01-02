#define _CRT_SECURE_NO_WARNINGS
#ifndef DB_UTILS_H
#define DB_UTILS_H
int init_db();
void close_db();
int sort_students(int sort_type);
int delete_student_by_stuId(const char* stuId);
int query_student_by_stuId(const char* stuId);
int query_callback(void* data, int argc, char** argv, char** azColName);
int insert_student(
	 const char* stuId
	,const char* name
	,const char* gender
	,const char* identify
	,const char* major
	,const char* teleNumber
	,const char* stuClass
	,const char* age
	,const char* sit
	,const char* stuId_2
	,const char* examId
);
int dbSortByType(int tpye);


#endif	