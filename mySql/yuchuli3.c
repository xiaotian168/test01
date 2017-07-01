#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

//预处理查询    select语句

int main()
{
    MYSQL *mysql = mysql_init(NULL);

    mysql_real_connect(mysql, "127.0.0.1", "ls", "itcast", "test", 3306, NULL, 0);
    if(NULL == mysql)
    {
        printf("mysql_real_connect error\n");
        return -1;
    }
    
    if(mysql_query(mysql, "set names utf8"))    //设置utf8编码
    {
        printf("set names utf8 error\n");
        return -2;
    }
    
    //初始化数据库句柄
    MYSQL_STMT *stmt = mysql_stmt_init(mysql);
    char *sql = "select * from tab1 where id=?";
    if(mysql_stmt_prepare(stmt,sql,strlen(sql)))   //绑定sql预处理语句到数据库句柄
    {
        printf("sql 参数绑定出错\n");
        return -3;
    }
    //绑定?号参数
    int condition = 1;
    MYSQL_BIND condition_bind;
    condition_bind.buffer_type = MYSQL_TYPE_LONG;
    condition_bind.buffer = (void*)&condition;
    condition_bind.buffer_length = sizeof(condition);
    condition_bind.is_null = (my_bool*)0;
    mysql_stmt_bind_param(stmt, &condition_bind);
    //初始化每个字段结构体数据
    int id;
    char name[1024];
    unsigned long name_length =(unsigned long)sizeof(name);
    int age;
    MYSQL_BIND bind[3];
    memset(bind, 0, sizeof(bind)); //bind作为输出；需要清空每个结构体数据先 
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (void*)&id;
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)name;
    bind[1].buffer_length = name_length;
    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = (void*)&age;
    //绑定字段结构体数组到数据库句柄中
    mysql_stmt_bind_result(stmt, bind);
    //执行绑定好的预处理语句
    mysql_stmt_execute(stmt);

    //执行完预处理后获取select查询结果
    mysql_stmt_store_result(stmt);  //执行获取预处理查询结果集动作
    int ret;
    while(!(ret = mysql_stmt_fetch(stmt))) //获取结果集中的每行结果并且判断输入
    {
        printf("%d\t",id);
        printf("%s\t",name);
        printf("%d\t",age);
        printf("\n");
    }
    mysql_stmt_free_result(stmt);   

    mysql_stmt_close(stmt);
    mysql_close(mysql);
    return 0;
}

