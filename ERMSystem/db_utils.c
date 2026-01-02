#define _CRT_SECURE_NO_WARINGS
#include "db_utils.h"
#include<stdint.h>
#include <stdio.h>
#include<string.h>
#include"sqlite3.h"
static sqlite3* db = NULL;//定义空指针,保证链接数据库的唯一性,解耦合
// 1. 初始化数据库
int init_db() {
    int ret;
    char* err_msg;
    // 打开/创建数据库文件
    ret = sqlite3_open("student.db", &db);
    if (ret != SQLITE_OK) {
        printf("数据库打开失败: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    // 创建学生表：id(主键),学号,姓名,性别,身份证,专业,手机号,座位号,学号前两位,准考证号
    const char* create_sql = "CREATE TABLE IF NOT EXISTS student ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"//暂时不管他
        "stuId TEXT UNIQUE NOT NULL,"
        "name TEXT NOT NULL,"
        "gender TEXT,"//排查10分钟😡
        "identify TEXT,"
        "major TEXT,"
        "teleNumber TEXT,"
        "sit TEXT,"
        "age TEXT,"
        "stuId_2 TEXT,"
        "examId TEXT"
        ");";
    ret = sqlite3_exec(db, create_sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        printf("建表失败: %s 请删除db文件\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }
    printf("数据库初始化成功！\n");
    return 0;
}
int insert_student(
      const char* stuId
    , const char* name
    , const char* gender
    , const char* identify
    , const char* major
    , const char* teleNumber
    , const char* sit
    , const char* age
    , const char* stuId_2
    , const char* examId) {
    if (db == NULL) {
        printf("数据库未初始化，请先调用init_db()\n");
        return -1;
    }

    // 拼接插入SQL（使用参数绑定避免SQL注入，更安全）
    char insert_sql[1024] = { 0 };
    snprintf(insert_sql, sizeof(insert_sql),
        "INSERT INTO student (stuId, name, gender, identify, major, teleNumber, sit , age , stuId_2, examId) "
        "VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');",
        stuId, name, gender, identify, major, teleNumber, sit, stuId_2, examId);

    int ret;
    char* err_msg = NULL;
    ret = sqlite3_exec(db, insert_sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        printf("插入学生信息失败: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    printf("学生信息插入成功！\n");
    return 0;
}
static int query_callback(void* data, int argc, char** argv, char** azColName) {
    // 打印一行查询结果
    printf("-------------------------\n");
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("-------------------------\n");
    return 0;
}
static int callback_get_stuId2(void* data, int argc, char** argv, char** azColName) {
    // data是传入的stuId_2数组，将查询到的stuId_2值复制进去
    char* stuId_2 = (char*)data;
    if (argv[0] != NULL) {
        strncpy(stuId_2, argv[0], 2); // 只取前两位（假设stuId_2是2位）
    }
    return 0;
}
int query_all_students() {
    if (db == NULL) {
        printf("数据库未初始化，请先调用init_db()\n");
        return -1;
    }

    const char* query_sql = "SELECT * FROM student;";
    int ret;
    char* err_msg = NULL;

    ret = sqlite3_exec(db, query_sql, query_callback, NULL, &err_msg);
    if (ret != SQLITE_OK) {
        printf("查询学生信息失败: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    printf("学生信息查询完成！\n");
    return 0;
}

int query_student_by_stuId(const char* stuId) {
    if (db == NULL) {
        printf("数据库未初始化，请先调用init_db()\n");
        return -1;
    }

    char query_sql[256] = { 0 };
    snprintf(query_sql, sizeof(query_sql),
        "SELECT * FROM student WHERE stuId = '%s';", stuId);

    int ret;
    char* err_msg = NULL;
    ret = sqlite3_exec(db, query_sql, query_callback, NULL, &err_msg);
    if (ret != SQLITE_OK) {
        printf("查询学生信息失败: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    printf("学生信息查询完成！\n");
    return 0;
}
int delete_student_by_stuId(const char* stuId) {
    if (db == NULL) {
        printf("数据库未初始化，请先调用init_db()\n");
        return -1;
    }

    char delete_sql[256] = { 0 };
    snprintf(delete_sql, sizeof(delete_sql),
        "DELETE FROM student WHERE stuId = '%s';", stuId);

    int ret;
    char* err_msg = NULL;
    ret = sqlite3_exec(db, delete_sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        printf("删除学生信息失败: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    // 检查是否有行被删除
    int changes = sqlite3_changes(db);
    if (changes == 0) {
        printf("未找到学号为%s的学生，无数据删除\n", stuId);
        return -1;
    }

    printf("学生信息删除成功！\n");
    return 0;
}
int update_student_info(const char* stuId, const char* new_col, const char* new_val) {
    if (db == NULL) return -1;
    char update_sql[256] = {0};
    snprintf(update_sql, sizeof(update_sql), "UPDATE student SET %s = '%s' WHERE stuId = '%s';", new_col, new_val, stuId);
    char* err_msg = NULL;
    int ret = sqlite3_exec(db, update_sql, NULL, 0, &err_msg);
    if (ret != SQLITE_OK) {
        printf("更新失败: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    if (sqlite3_changes(db) == 0) {
        printf("未找到该学生！\n");
        return -1;
    }
    return 0;
}
// sort_type: 1-学号升序  2-准考证号升序  3-年龄升序
int sort_students(int sort_type) {
    if (db == NULL) {
        printf("数据库未初始化，请先调用init_db()\n");
        return -1;
    }

    char sort_sql[1024] = { 0 };
    switch (sort_type) {
    case 1:
        snprintf(sort_sql, sizeof(sort_sql), "SELECT * FROM student ORDER BY stuId ASC;");
        break;
    case 2:
        snprintf(sort_sql, sizeof(sort_sql), "SELECT * FROM student ORDER BY examId ASC;");
        break;
    case 3:
        snprintf(sort_sql, sizeof(sort_sql), "SELECT * FROM student ORDER BY age ASC;");
        break;
        return -1;
    }

    int ret;
    char* err_msg = NULL;
    ret = sqlite3_exec(db, sort_sql, query_callback, NULL, &err_msg);
    if (ret != SQLITE_OK) {
        printf("排序查询失败: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    printf("排序查询完成！\n");
    return 0;
}

//int assign_exam_id(const char* stuId) {
//    if (db == NULL) {
//        printf("数据库未初始化！\n");
//        return -1;
//    }
//    char check_sql[256] = { 0 };
//    snprintf(check_sql, sizeof(check_sql), "SELECT stuId_2 FROM student WHERE stuId = '%s';", stuId);
//
//    char* err_msg = NULL;
//    char stuId_2[3] = { 0 }; // 学号前两位
//    
//
//    int ret = sqlite3_exec(db, check_sql, callback_get_stuId2, stuId_2, &err_msg);
//    if (ret != SQLITE_OK) {
//        printf("查询学号前两位失败: %s\n", err_msg);
//        sqlite3_free(err_msg);
//        return -1;
//    }
//
//    if (strlen(stuId_2) == 0) {
//        printf("未找到学号为%s的学生！\n", stuId);
//        return -1;
//    }
//
//    // 2. 计算考场号和座位号
//    int total_count = get_student_count();
//    if (total_count <= 0) {
//        printf("学生数量统计异常！\n");
//        return -1;
//    }
//
//    const char* school_code = "01"; // 固定学校代码
//    int exam_room = (total_count - 1) / 40 + 1; // 每40人一个考场（1-40→01，41-80→02...）
//    int seat_num = (total_count - 1) % 40 + 1;  // 考场内座位号（1-40）
//
//    // 3. 格式化考场号和座位号（补零为2位）
//    char exam_room_str[3] = { 0 };
//    char seat_num_str[3] = { 0 };
//    snprintf(exam_room_str, sizeof(exam_room_str), "%02d", exam_room);
//    snprintf(seat_num_str, sizeof(seat_num_str), "%02d", seat_num);
//
//    // 4. 拼接8位准考证号
//    char exam_id[9] = { 0 }; // 8位+结束符
//    snprintf(exam_id, sizeof(exam_id), "%s%s%s%s",
//        exam_room_str, school_code, stuId_2, seat_num_str);
//
//    // 5. 更新数据库中的准考证号
//    ret = update_student_info(stuId, "examId", exam_id);
//    if (ret != 0) {
//        printf("分配准考证号失败！\n");
//        return -1;
//    }
//
//    printf("准考证号分配成功！学号：%s → 准考证号：%s\n", stuId, exam_id);
//    return 0;
//}
void close_db() {
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
        printf("数据库连接已关闭\n");
    }
}
