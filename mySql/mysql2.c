#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main()
{
    //创建mysql变量
    MYSQL *mysql = mysql_init(NULL);
    //连接, 连接获取mysql值
    mysql = mysql_real_connect(mysql, "127.0.0.1", "root", "itcast", "test", 3306, NULL, CLIENT_MULTI_STATEMENTS);    //最后参数为状态, 这里表示设置sql语句可执行多条
    if(NULL == mysql)
    {
        printf("connect error\n");
        return -1;
    }

    if(mysql_query(mysql, "set names utf8")) //设置utf8格式插入；解决乱码问题
    {
        printf("query 设置utf8插入格式失败\n");
        return -2;
    }

    char *sql = "insert into test4 values(5, '王麻子', 2); insert into test4 values(6, '张胖子', 2)";
    if(mysql_query(mysql, sql))
    {
        printf("query error\n");
        return -3;
    }



    //关闭连接
    mysql_close(mysql);
    return 0;
}
