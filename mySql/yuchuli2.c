#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

//预处理：只解析命令一次；可以执行多次（更加参数不同）
//减少mysql的命令的解析次数；提高效率

//预处理查询；注意查询语句中必须要有?才行

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

/*
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
*/
    
    char *sql = "select * from test2 where classId=?";
    mysql_stmt_prepare(stmt,sql,strlen(sql));   //绑定查询语句
    //绑定?号参数
    int condition = 1;
    MYSQL_BIND bind_condition;
    bind_condition.buffer_type = MYSQL_TYPE_LONG;
    bind_condition.buffer = (void*)&condition;
    bind_condition.buffer_length = sizeof(condition);
    bind_condition.is_null = (my_bool *)0;
    mysql_stmt_bind_param(stmt, &bind_condition);   //绑定结构体参数到句柄上

    int id;
    char name[256];
    int classId;

    MYSQL_BIND bind[3];
    memset(bind, 0, sizeof(bind));      //作为绑定结果参数；要清0先
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (void*)&id;
    
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)name;
    bind[1].buffer_length = sizeof(name);
    
    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = (void*)&classId;
    
    mysql_stmt_bind_result(stmt, bind); //绑定字段数据信息
    mysql_stmt_execute(stmt);   //执行与语句句柄相关的预处理查询
    
    
    //5.输出查询结果到屏幕
    //获取查询结果集
    mysql_stmt_store_result(stmt);
    int ret;
    while(!(ret = mysql_stmt_fetch(stmt)))  //每次输出一行
    {
        printf("%d\t", id);
        printf("%s\t", name);
        printf("%d\t", classId);
        printf("\n");
    }

    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);

    //关闭连接
    mysql_close(mysql);
    return 0;
}

