#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

//注意：预处理的查询语句需要?；否则查询不出来

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


//char *sql = "select * from test4";  //error 没有?
char *sql = "select * from test4 whert id = ?";
mysql_stmt_prepare(stmt, sql, stlen(sql));

//字段数据初始化；并且绑定字段集合



mysql_stmt_bind_result(stmt,bind);  //加入绑定字段结构体到数据库句柄结构体
mysql_stmt_execute(stmt);
mysql_stmt_store_result(stmt);
while(!mysql_stmt_fetch(stmt))
{
    printf();   //输出结果集
    printf();
    printf();

}

//返回的结构体需要初始化；不然会段错误
