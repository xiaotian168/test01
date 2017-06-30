#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main()
{
    MYSQL *mysql = mysql_init(NULL);

    mysql = mysql_real_connect(mysql, "127.0.0.1",);

    mysql_query(mysql, "set names utf8");

    while(1)
    {
        printf("mysql >");
        char sql[1024] = {0};
        fgets(sql, 1024, stdin);
        if(strncmp("insert", sql, 6) == 0 || strncmp("update")
                || strncmp("delete")
                {
                    if(mysql_query(mysql, sql))
                    {
                        
                    }

                    int row = mysql_affected_rows(mysql);
                    printf();
                }
                else if(strncmp("select")
                    {
                    if(mysql_query(mysql, sql))
                    {
                        
                    }
                        MYSQL_RES *result = mysql_store_result(mudl);
                        int count = mysql_num_fields(result);
                        MYSQL_FIELD *fields = mysql_fetch_fields(result);
                        for()
                        {
                            printf()    //fields[i].name
                        }


                        MYSQL_ROW row;
                        while(row = mysql_fetch_row(result))
                        {
                            for()
                            {
                                printf();   //row[i];
                            }
                        }

                        int rows = mysql_num_rows(result);
                        printf();
                        //释放
                        mysql_free_result(result);
                    }
        else if()   //退出
        {
        }
    }
}
