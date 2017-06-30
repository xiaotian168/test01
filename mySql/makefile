all:out
out:mysql1.c mysql2.c mysql3.c select.c select.c mysqlTools.c yuchuli.c yuchuli2.c
	gcc mysql1.c -o mysql1.out -I/usr/include/mysql -lmysqlclient
	gcc mysql2.c -o mysql2.out -I/usr/include/mysql -lmysqlclient
	gcc mysql3.c -o transaction.out -I/usr/include/mysql -lmysqlclient
	gcc select.c -o select.out -I/usr/include/mysql -lmysqlclient -std=c99
	gcc mysqlTools.c -o mysql.out -I/usr/include/mysql -lmysqlclient -std=c99
	gcc yuchuli.c -o yuchuli.out -I/usr/include/mysql -lmysqlclient -std=c99
	gcc yuchuli2.c -o yuchuli2.out -I/usr/include/mysql -lmysqlclient -std=c99


