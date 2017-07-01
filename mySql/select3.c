#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

//select 查询

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
        printf("设置utf8编码错误\n");
        return -2;
    }

    //select语句执行
    char *sql = "select * from tab2";
    if(mysql_query(mysql, sql))
    {
        printf("select 执行失败\n");
        return -3;
    }

    MYSQL_RES *result = mysql_store_result(mysql);
    if(NULL == result)
    {
        printf("未返回结构集\n");
        return -4;
    }

    //打印字段信息
    unsigned int num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    for(int i = 0; i < num_fields; i++)
    {
        //printf("Field %u is %s\n", i, fields[i].name);
        printf("%s\t", fields[i].name);
    }
    printf("\n");

    //打印每行记录信息
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s\t", row[i]);
        }
        printf("\n");
    }

    mysql_free_result(result);
    mysql_close(mysql);
}   
