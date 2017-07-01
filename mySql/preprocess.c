#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

//预处理
int main()
{
    MYSQL *mysql = mysql_init(NULL);

    mysql_real_connect(mysql,"127.0.0.1","root","itcast","test",3306,NULL,0);
    if(NULL == mysql)
    {
        printf("mysql_real_connect error\n");
        return -1;
    }

    if(mysql_query(mysql, "set names utf8"))
    {
        printf("set names utf8 出错\n");
        return -2;
    }


    MYSQL_STMT *stmt = mysql_stmt_init(mysql);
    if(NULL == stmt)
    {
        printf("mysql_stmt_init error\n");
        return -3;
    }

    char *sql = "insert into test2 values(?,?,?)";
    mysql_stmt_prepare(stmt, sql, strlen(sql));

    MYSQL_BIND bind[3];
    int id=0;
    char name[1024] = {0};
    int classId = 0;
    bind[0].buffer_type = 
    mysql_stmt_close(stmt);
    mysql_close(mysql);
}
