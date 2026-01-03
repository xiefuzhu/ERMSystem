#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

int num = 1;
int totalnum=1;
void menu() {
    //cszyy
    int choice;

loop:
    while (1) {
        system("cls");
        printf("\n**********考生报名管理系统**********");
        printf("\n1.考生信息录入");
        printf("\n2.考生信息修改");
        printf("\n3.分配准考证号");
        printf("\n4.按学号查询考生信息");
        printf("\n5.考生排序");
        printf("\n0.退出系统");
        printf("\n提示:请输入数字序号选择对应的操作！\n");
        scanf("%d", &choice);
        while (getchar() != '\n');
        switch (choice) {
        case 1:
            system("cls");
            lu();

            break;
        case 2:
            system("cls");
            char id[20];
            printf("请输入修改学生学号");
            scanf("%s" ,id);
            query_student_by_stuId(id);
            update_student_info();
            break;
        case 3:
            system("cls");
            printf("分配准考证号");
            break;
        case 4:
            system("cls");
            printf("按学号查询考生信息");
            break;
        case 5:

            system("cls");
            int choice2;
            int iuputYes;
            int inputYes = 0;
        loop2:
            while (!inputYes) {
                printf("1.按考生学号进行升序排序\n2.按准考证号进行升序排序\n3.按考生年龄升序排序\n4.返回上一级");//此处需要插入函数 
                scanf("%d", &choice2);
                if (choice2 == 1) {
                    system("cls");
                    inputYes = 1;
                    printf("按考生学号进行升序排序\n");
                    //hanshu
                    sort_students(1);
                    printf("\n按回车键返回主菜单");

                    getchar();
                    getchar();


                }
                else if (choice2 == 2) {
                    system("cls");
                    inputYes = 1;
                    printf("按考生准考证号进行升序排序\n");
                    //hanshu
                    sort_students(2);
                    printf("\n按回车键返回主菜单");
                    getchar();
                    getchar();




                }
                else if (choice2 == 3) {
                    system("cls");
                    inputYes = 1;
                    printf("按考生年龄升序排序\n");
                    //hanshu
                    sort_students(3);
                    printf("\n按回车键返回主菜单");
                    getchar();
                    getchar();



                }
                else if (choice2 == 4) {
                    system("cls");
                    inputYes = 1;
                    goto loop;//返回menu
                }
                else {
                    system("cls");
                    printf("输入无效，请重新选择！\n");


                    goto loop2;
                }
            }
            break;
        case 0:
            system("cls");
            printf("感谢您的使用");
            exit(0);
        default:
            system("cls");
            printf("输入无效，请重新选择！");
            goto loop;
        }
    }
}

int main() {
    if (init_db() != 0) {
        return -1;
    }
    menu();
    return 0;
}
int lu() {
    printf("考生信息录入\n");
    char stuId[50] = { 0 };
    char name[50] = { 0 };
    char gender[20] = { 0 };
    char identify[50] = { 0 };
    char major[50] = { 0 };
    char teleNumber[50] = { 0 };
    char sit[3] = { 0 };
    char age[20] = { 0 };
    char stuId_2[3] = { 0 };
    char classNum[3] = { 0 };
    char examId[50] = { 0 };

    printf("请输入考生学号：");
    scanf("%s", stuId);
    while (getchar() != '\n');

    printf("请输入考生姓名：");
    scanf("%s", name);
    while (getchar() != '\n');

    printf("请输入考生性别：");
    scanf("%s", gender);
    while (getchar() != '\n');

    printf("请输入考生身份证号：");
    scanf("%s", identify);
    while (getchar() != '\n');

    printf("请输入考生专业：");
    scanf("%s", major);
    while (getchar() != '\n');

    printf("请输入考生联系电话：");
    scanf("%s", teleNumber);
    while (getchar() != '\n');

    //printf("请输入考生考场号：");
    //scanf("%s", sit);
    //while (getchar() != '\n');
    if (num < 10) {
        sit[0] = '0';
        sit[1] = (char)(num + 48);  
        sit[2] = '\0';              
    }
    else {
        sit[0] = (char)((num / 10) + 48);  // 十
        sit[1] = (char)((num % 10) + 48);  // 个
        sit[2] = '\0';                     
    }

    printf("请输入考生年龄：");
    scanf("%s", age);
    while (getchar() != '\n');

    //scanf("%s", stuId_2);学生号前两位
    //while (getchar() != '\n');

    //printf("请输入考生准考证号：");
    //scanf("%s", examId);
    //while (getchar() != '\n');
    stuId_2[0] = stuId[0];
    stuId_2[1] = stuId[1];
    examId[0] = '0';
    examId[1] = (char)((totalnum / 40) + 49);
    examId[2] = '0';
    examId[3] = '1';
    examId[4] = stuId[0];
    examId[5] = stuId[1];
    examId[6] = sit[0];
    examId[7] = sit[1];
    examId[8] = '\0';



    int ret = insert_student(stuId, name, gender, identify, major,
        teleNumber, sit, age, stuId_2, examId);

    // 插入结果提示
    if (!ret) {
        printf("考生信息录入成功！\n是否继续录入?输入回车继续录入,输入0返回上一级");
        num++;
        totalnum++;
    }
    else {
        printf("考生信息录入失败！\n");
    }

    // 等待用户确认后返回菜单
    printf("按回车键返回主菜单...");
    if(getchar()=='\n') lu();

}