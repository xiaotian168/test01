#include <stdio.h>
#include <mysql.h>

int main()
{
    //创建
    MYSQL *mysql = mysql_init(NULL);
    //连接, 获得mysql值
    mysql = mysql_real_connect(mysql, "127.0.0.1", "root", "itcast", "test", 3306, NULL, 0);
    if(mysql == NULL)   //判断是否连接成功
    {
        printf("connect error\n");
        return -1;
    }

    char *sql = "insert into test4 values(4,'xiaolin',1)";  //要执行的sql语句
    if(mysql_query(mysql, sql)) //判断是否执行成功
    {
        printf("query error\n");
        return -2;
    }

    //关闭mysql连接 
    mysql_close(mysql);
    return 0;
}
