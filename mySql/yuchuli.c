#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

//预处理：只解析命令一次；可以执行多次（更加参数不同）
//减少mysql的命令的解析次数；提高效率

int main()
{
    //1.创建
    MYSQL *mysql = mysql_init(NULL);
    //2.连接
    mysql = mysql_real_connect(mysql, "127.0.0.1", "root", "itcast", "test", 3306, NULL, 0);
    if(NULL == mysql)
    {
        printf("connect error\n");
        return -1;
    }

    //3.设置utf8编码
    if(mysql_query(mysql, "set names utf8"))
    {
        printf("设置utf8编码出错\n");
        return -2;
    }

    //4.设置数据库句柄
    MYSQL_STMT *stmt = mysql_stmt_init(mysql);  //初始化句柄数据库结构体
    if(NULL == stmt)
    {
        printf("mysql_stmt_init error\n");
        return -3;
    }

    char *sql = "insert into test2 values(?,?,?)";
    mysql_stmt_prepare(stmt,sql,strlen(sql));   //绑定执行语句(sql语句)到结构体stmt

    MYSQL_BIND bind[3];     //4表示字段个数;  绑定字段的数据结构体
    int id = 10;
    char name[1024] = {0};
    unsigned long name_length; //长度  求字符长度
    int classId = 1;

    //为每个字段赋值
    for(int i = 0; i < 3; ++i)
    {
        id += 1;
        sprintf(name, "%s%d", "Angel",i);
        classId = i;
        name_length = (unsigned long)strlen(name);
        bind[0].buffer_type = MYSQL_TYPE_LONG;
        bind[0].buffer = (void*)&id;
        bind[0].buffer_length = sizeof(id);
        bind[0].is_null = (my_bool*)0;
        bind[1].buffer_type = MYSQL_TYPE_STRING;
        bind[1].buffer = (void*)name;
        bind[1].buffer_length = strlen(name);
        bind[1].length = &name_length;
        bind[1].is_null = (my_bool*)0;
        bind[2].buffer_type = MYSQL_TYPE_LONG;
        bind[2].buffer = (void*)&classId;
        bind[2].buffer_length = sizeof(classId);
        bind[2].is_null = (my_bool*)0;

        //初始化数据后绑定到stmt数据库句柄结构体中
        mysql_stmt_bind_param(stmt, bind);
        if(mysql_stmt_execute(stmt))    //执行预处理语句
        {
            printf("execute error\n");
            return -4;
        }
    }

    //关闭连接
    mysql_close(mysql);
    return 0;

#if 0
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
#endif
}

