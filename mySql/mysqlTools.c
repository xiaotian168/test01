#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

//仿照mysql执行sql界面

int main()
{
    //1.创建mysql
    MYSQL *mysql = mysql_init(NULL);
    //2.连接
    mysql = mysql_real_connect(mysql, "127.0.0.1", "root", "itcast", "test", 3306, NULL, 0);
    if(NULL == mysql)
    {
        printf("connect error\n");
        return -1;
    }
    //3.设置utf8编码格式
    if(mysql_query(mysql, "set names utf8"))
    {
        printf("set names utf8 error\n");
        return -2;
    }

    //仿mysql登录进入查询界面
    while(1)
    {
        printf("mysql >");
        char sql[1024] = {0};
        fgets(sql, 1024, stdin);
        if(strncmp("insert", sql, 6) == 0 || strncmp("update", sql, 6) == 0 || strncmp("delete", sql, 6) == 0)  //支持增／删／改
        {
            if(mysql_query(mysql, sql)) //查询失败；继续
            {
                printf("query error\n");
                continue;
            }

            //查询成功；打印信息
            int row = mysql_affected_rows(mysql);   //获取更改变动的行数
            printf("Query OK, %d row affected\n", row);
        }
        else if(strncmp("select", sql, 6) == 0)  //查询
        {
            printf("in select ok\n");
            if(mysql_query(mysql, sql))
            {
                printf("select error\n");
                continue;
            }
            //获取查询结果
            MYSQL_RES *result = mysql_store_result(mysql);  //获取结果集
            int count = mysql_num_fields(result);   //获取结果集中的字段数 -->表头
            printf("cols count %d\n", count);
            MYSQL_FIELD *fields = mysql_fetch_fields(result);   //获取结果集中每列的结构数组 -->字段结构数组
            for(int i = 0; i < count; ++i)   //遍历打印字段(列)的值
            {
                printf("%s\t", fields[i].name);    //fields[i].name -->字段值
            }
            printf("\n");

            //获取行数据结构
            MYSQL_ROW row;
            while(row = mysql_fetch_row(result))    //自动遍历每行数据
            {
                for(int i = 0; i < count; ++i)  //打印每行中字段值
                {
                    printf("%s\t", row[i]);   //row[i];
                }
                printf("\n");
            }

            //获取总记录数；
            int rows = mysql_num_rows(result);
            printf("%d rows in set\n", rows);

            //释放结果结构体
            mysql_free_result(result);
        }
        else if(strncmp("exit", sql, 4) == 0)  //退出
        {
            break;
        }
    }

    //关闭连接
    mysql_close(mysql);
    return 0;
}

