#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main()
{
    MYSQL *mysql = mysql_init(NULL);

    mysql = mysql_real_connect(mysql, "127.0.0.1", "root", "itcast", "test", 3306, NULL, 0);

    //设置编码格式
    mysql_query(mysql, "set names utf8");

    //执行查询语句
    mysql_query(mysql, "select * from test4");

    //检索结果集
    MYSQL_RES *result = mysql_store_result(mysql);
    //获取结果集中的字段数
    unsigned int count = mysql_num_fields(result);
    printf("count = %d\n", count);
    //获取表头,字段值
    for(int i = 0; i < count; ++i)
    {
        MYSQL_FIELD *field = mysql_fetch_field(result);
        printf("%s\t", field->name);
    } 
    printf("\n");

    //获取结果集中行的数据
    MYSQL_ROW row;
    while(row = mysql_fetch_row(result))
    {
        for(int i = 0; i<count; ++i)
        {
            printf("%s\t", row[i]);
        }
        printf("\n");
    }
    

    //释放结果集
    mysql_free_result(result);
    //释放连接
    mysql_close(mysql);
    return 0;
}
