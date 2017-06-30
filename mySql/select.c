#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

//查询表中数据；并将结果输出打印到屏幕

int main()
{
    //1.创建mysql
    MYSQL *mysql = mysql_init(NULL);
    //2.连接数据库；赋值结果给mysql
    mysql = mysql_real_connect(mysql, "127.0.0.1", "root", "itcast", "test", 3306, NULL, 0);
    if(NULL == mysql)
    {
        printf("connect error\n");
        return -1;
    }

    //3.设置编码格式    -->0表示执行成功
    if(mysql_query(mysql, "set names utf8"))
    {
        printf("编码格式设置出错\n");
        return -2;
    }


    //4.执行查询语句
    if(mysql_query(mysql, "select * from test4"))
    {
        printf("select error\n");
        return -3;
    }

    //5.获取结果；并输出到屏幕
    //检索结果集; 获取结果结构体
    MYSQL_RES *result = mysql_store_result(mysql);
    //从结果中获取结果集中的字段数  -->分解得到结果集中单个数据信息
    unsigned int count = mysql_num_fields(result);
    printf("count = %d\n", count);
    //获取表头,字段值
    for(int i = 0; i < count; ++i)  //获取打印没个字段值
    {
        MYSQL_FIELD *field = mysql_fetch_field(result);
        printf("%s\t", field->name);
    } 
    printf("\n");   //换行

    //获取结果集中行的数据
    MYSQL_ROW row;
    while(row = mysql_fetch_row(result))    //自动读取每行数据
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
