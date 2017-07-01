#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

//执行普通查询结果打印；

int main()
{
    MYSQL *mysql = mysql_init(NULL);

    mysql_real_connect(mysql, "127.0.0.1", "ls","itcast","test",3306,NULL,0);
    if(NULL == mysql)
    {
        printf("mysql_real_connect error\n");
        return -1;
    }

    if(mysql_query(mysql, "set names utf8"))
    {
        printf("设置utf8编码失败\n");
        return -2;
    }

    if(mysql_query(mysql, "select * from tab1"))
    {
        printf("select error\n");
        return -3;
    }

    //获取查询结果集
    MYSQL_RES *result = mysql_store_result(mysql);
    if(NULL == result)
    {
        printf("获取查询结果集错误\n");
        return -3;
    }
    //获取结果集中的列数(字段数量)
    unsigned int cols = mysql_num_fields(result);
    printf("cols = %d\n", cols);
    //获取结果集中的字段值
    for(int i = 0; i < cols; ++i)
    {
        MYSQL_FIELD *field = mysql_fetch_field(result); //每次自动获取一行
        printf("%s\t",field->name);     //打印字段值
    }
    printf("\n");
    //获取结果集中的每行数据信息
    MYSQL_ROW row;
    while(row = mysql_fetch_row(result))
    {
        for(int i = 0; i < cols; ++i)
        {
            printf("%s\t", row[i]); //打印每行的数据信息
        }
        printf("\n");
    }

    mysql_free_result(result);
    mysql_close(mysql);
    return 0;
}

