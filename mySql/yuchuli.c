#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main()
{
    MYSQL *mysql = mysql_init(NULL);

    mysql = mysql_real_connect();

    mysql_query(sql, "set names utf8");

    MYSQL_SIMT *stmt = mysql_stmt_init(mysql);  //初始化句柄数据库结构体
    char *sql = "insert into test4 values(?,?,?,?)";
    mysql_stmt_prepare(stmt,sql,strlen(sql));   //绑定执行语句(sql语句)到结构体stmt

    MYSQL_BIND bind[4];     //4表示字段个数;  绑定字段的数据结构体
    int id = 10;
    char *name = "abc";
    unsigned long name_length = (unsigned long)strlen(name)//长度  求字符长度
    int age =20;
    int classId = 1;

    //为每个字段赋值
    bind[0].buffer_type = MYSQL_TYPE_LONG;  //指定类型
    bind[0].buffer = (void *)&id;   //初始化数据
    bind[0].buffer_length = sizeof(id); //长度
    bind[0].is_null = (my_bool *)0;     //

    //字符串类型
    bind[1].buffer_type = MYSQL_TYPE_LONG;  //指定类型
    bind[1].buffer = (void *)&id;   //初始化数据
    bind[1].buffer_length = sizeof(id); //长度
    bind[1].is_null = (my_bool *)0;     //
    bind[1].length = &name_length;  //字符串类型要多一步；bind长度


    mysql_stmt_bind_param(stmt, bind);  //绑定字段结构体数据到句柄数据库结构体中
    if(mysql_stmt_execute(stmt))   //执行预处理
    {
        printf("execute error");
        return -1;
    }

    mysql_close(mysql);
    return 0;
}
