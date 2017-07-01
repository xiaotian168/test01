#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

//预处理

int main()
{
    MYSQL *mysql = mysql_init(NULL);

    mysql_real_connect(mysql, "127.0.0.1", "ls", "itcast", "test", 3306, NULL, 0);
    if(NULL == mysql)
    {
        printf("mysql_connect error\n");
        return -1;
    }

    if(mysql_query(mysql, "set names utf8"))
    {
        printf("mysql_query error\n");
        return -2;
    }

    //数据库句柄
    MYSQL_STMT *stmt = mysql_stmt_init(mysql);  //获得句柄
    char *sql = "insert into tab1 values(?,?,?)";
    mysql_stmt_prepare(stmt, sql, strlen(sql)); //绑定sql给数据库句柄

    //初始化各个字段数据结构体
    int id = 1;
    char *name = "张胖子";
    unsigned long name_length = (unsigned long)strlen(name);
    int age = 18;
    MYSQL_BIND bind[3];     //表tab1中只有3个字段；
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (void *)&id;
    bind[0].buffer_length = sizeof(id);
    bind[0].is_null = (my_bool*)0;
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void *)name;
    bind[1].buffer_length = sizeof(name); 
    bind[1].length = &name_length;
    bind[1].is_null = (my_bool*)0;
    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = (void *)&age;
    bind[2].buffer_length = sizeof(age);
    bind[2].is_null = (my_bool*)0;
    //绑定各字段数据结构体到数据库句柄
    mysql_stmt_bind_param(stmt, bind);
    //执行预处理
    mysql_stmt_execute(stmt);

    mysql_stmt_close(stmt);
    mysql_close(mysql);
    return 0;
}
