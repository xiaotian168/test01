#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

//事务
//执行多条sql的API

int main()
{
    //1.创建mysql变量
    MYSQL *mysql = mysql_init(NULL);
    //2.连接, 连接获取mysql值
    mysql = mysql_real_connect(mysql, "127.0.0.1", "root", "itcast", "test", 3306, NULL, CLIENT_MULTI_STATEMENTS);    //最后参数为状态, 这里表示设置sql语句可执行多条
    if(NULL == mysql)
    {
        printf("connect error\n");
        return -1;
    }

    if(mysql_query(mysql, "set names utf8")) //3.设置utf8格式插入；解决乱码问题
    {
        printf("query 设置utf8插入格式失败\n");
        return -2;
    }

    //4.执行sql语句；开始事务(保证后面操作的原子性)
    if(mysql_query(mysql, "start transaction"))
    {
        printf("start transaction");
    }

    
    //5.保存多条sql执行结果，作为判断和回滚条件变量
    int ret1 = mysql_query(mysql, "insert into test4 values(7,'xiaoxiao',1)");
    int ret2 = mysql_query(mysql, "insert into test4 values(8,'Jack',1)");
    if(ret1 || ret2)    //执行结果为0表示成功
    {
        //说明至少有一条sql语句为执行成功；执行回滚
        mysql_query(mysql, "rollback");
        printf("操作执行失败, 回滚\n");
        return -3;
    }
    if(mysql_query(mysql, "commit"))    //判断执行提交是否成功；保证服务器那边数据的更新
    {
    printf("3\n");
        printf("commit error\n");
        return -4;
    }
    

    printf("执行成功\n");
    //关闭连接
    mysql_close(mysql);
    return 0;
}
