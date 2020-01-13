 
// cgi_select.cpp : 定义控制台应用程序的入口点。
 
//
 
 
#include "stdafx.h"
 
#include <Windows.h>
 
#include <mysql.h>
 
#pragma comment(lib,"libmysql.lib")
 
using namespace std;
 
void start_meun()
{
	cout << "Connection to SQL\n";
	printf(" ---------------------------------------------------\n");
	printf("                 欢迎使用图书管理系统               \n");
	printf(" ---------------------------------------------------\n");
	printf("      [1].进入图书管理系统       [0].退出系统:");
}
 
void end_meun(MYSQL  *sock)
{
 
	mysql_close(sock);
 
	printf(" --------------------------------------------------\n");
	printf("       -=  感谢使用，再见！ =-           \n");
	printf(" --------------------------------------------------\n");
	system("pause");
	exit(0);
}
 
void login_meun()
{
	printf(" ---------------------------------------------------\n");
	printf("                选择用户类型登录系统                \n");
	printf("    [1].管理员登陆     [2].读者登陆     [0]退出系统    \n");
	printf(" ---------------------------------------------------\n");
}
 
void reader_meun()
{
	printf(" ----------------------------------------------------------------------\n");
	printf("                                      选择操作                \n");
	printf("    [1].查询图书     [2].查询借阅情况    [3].修改密码    [4].返回登陆界面    [0].退出系统    \n");
	printf(" ----------------------------------------------------------------------\n");
}
 
void admin_meun()
{
	printf(" --------------------------------------------------------------------\n");
	printf("                               选择操作                \n");
	printf("    [1].查询图书     [2].添加图书    [3].删除图书       \n");
	printf("    [4].查询用户     [5].添加用户    [6].删除用户    [0].退出系统    \n");
	printf(" --------------------------------------------------------------------\n");
}
 
void admin_op(MYSQL *sock,char *admin_id)
{
	while (1)
	{
		admin_meun();
		int a_op = 0;
		cin >> a_op;
		if (!a_op)
		{
			end_meun(sock);
		}
		else if (a_op == 1)
		{
			char sqlbuf[200] = "SELECT * FROM book";
			if (mysql_query(sock, sqlbuf))
			{
				printf("数据获取失败!:%s", mysql_error(sock));
				return;
			}
 
			MYSQL_RES *result;
			unsigned uLine = 0, uField = 0;
			MYSQL_ROW row;
			MYSQL_FIELD *pField(NULL);
 
			if (!(result = mysql_store_result(sock)))
			{
				printf("结果获取失败!:%s", mysql_error(sock));
				return;
			}
 
			//获得行数  
			uLine = (unsigned)mysql_num_rows(result);
 
			//获得字段数  
			uField = (unsigned)mysql_num_fields(result);
 
			//填充结果集 
			cout << "书本编号   书本类型    书本名称   书本封面   书本价格   书本出版社 书本作者\n";
			for (int i = 0; i<(int)uLine; i++)
			{
				//获得一行数据  
				row = mysql_fetch_row(result);
				for (int j = 0; j<(int)uField; j++)
				{
					printf("|%-10s", row[j]);
					//printf("       ");
				}
				printf("\n");
 
			}
			mysql_free_result(result);
		}
		else if (a_op == 2)
		{
			char new_book_id[20], new_book_name[20], new_book_face[20], new_book_publisher[20], new_book_writer[20];
			char new_book_price[20],new_book_type[20];
 
			cout << "输入书本id:";
			cin >> new_book_id;
			cout << "输入书本类型:";
			cin >> new_book_type;
			cout << "输入书本名字:";
			cin >> new_book_name;
			cout << "输入书本封面:";
			cin >> new_book_face;
			cout << "书本价格:";
			cin >> new_book_price;
			cout << "输入书本出版社:";
			cin >> new_book_publisher;
			cout << "输入书本作者:";
			cin >> new_book_writer;
 
			char sqlbuf[200] = "INSERT INTO book VALUES(";
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_book_id);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_book_type);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_book_name);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_book_face);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_book_price);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_book_publisher);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_book_writer);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ")");
 
			if (mysql_query(sock, sqlbuf))
			{
				printf("数据获取失败!:%s", mysql_error(sock));
				return;
			}
			mysql_commit(sock);
			cout << "添加成功!" << endl;
		}
		else if (a_op == 3)
		{
			char new_book_id[20];
 
			cout << "输入要删除书本的id:";
			cin >> new_book_id;
 
			char sqlbuf[200] = "DELETE FROM book WHERE book_id=";
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_book_id);
			strcat_s(sqlbuf, "'");
 
			if (mysql_query(sock, sqlbuf))
			{
				printf("数据获取失败!:%s", mysql_error(sock));
				return;
			}
			mysql_commit(sock);
			cout << "删除成功!" << endl;
		}
		else if (a_op == 4)
		{
			char sqlbuf[200] = "SELECT * FROM reader";
			if (mysql_query(sock, sqlbuf))
			{
				printf("数据获取失败!:%s", mysql_error(sock));
				return;
			}
 
			MYSQL_RES *result;
			unsigned uLine = 0, uField = 0;
			MYSQL_ROW row;
			MYSQL_FIELD *pField(NULL);
 
			if (!(result = mysql_store_result(sock)))
			{
				printf("结果获取失败!:%s", mysql_error(sock));
				return;
			}
 
			//获得行数  
			uLine = (unsigned)mysql_num_rows(result);
 
			//获得字段数  
			uField = (unsigned)mysql_num_fields(result);
 
			//填充结果集  
			cout << "用户id    管理者    用户类型    用户名称    用户密码   读者性别   电话        地址      创建时间    公司\n";
			for (int i = 0; i<(int)uLine; i++)
			{
				//获得一行数据  
				row = mysql_fetch_row(result);
				for (int j = 0; j<(int)uField; j++)
				{
					printf("|%-10s", row[j]);
				}
				printf("\n");
 
			}
			mysql_free_result(result);
		}
		else if (a_op == 5)
		{
			char new_reader_id[20], new_reader_name[20], new_reader_company[20], new_reader_work[20], new_reader_pass[20];
 
			cout << "输入读者id:";
			cin >> new_reader_id;
			cout << "输入读者名字:";
			cin >> new_reader_name;
 
			cout << "输入读者公司:";
			cin >> new_reader_company;
 
			cout << "输入读者工作:";
			cin >> new_reader_work;
 
			new_reader_pass[0] = '1';
			new_reader_pass[1] = '2';
			new_reader_pass[2] = '3';
			new_reader_pass[3] = '\0';
 
			char sqlbuf[200] = "INSERT INTO reader(reader_id,reader_name,reader_company,reader_work,reader_pass) VALUES(";
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_reader_id);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_reader_name);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_reader_company);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_reader_work);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ",");
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_reader_pass);
			strcat_s(sqlbuf, "'");
 
			strcat_s(sqlbuf, ")");
 
			cout << sqlbuf << endl;
			if (mysql_query(sock, sqlbuf))
			{
				printf("数据获取失败!:%s", mysql_error(sock));
				return;
			}
			mysql_commit(sock);
			cout << "添加成功!" << endl;
		}
		else if (a_op == 6)
		{
			char new_reader_id[20];
 
			cout << "输入要删除读者的id:";
			cin >> new_reader_id;
 
			char sqlbuf[200] = "DELETE FROM reader WHERE reader_id=";
 
			strcat_s(sqlbuf, "'");
			strcat_s(sqlbuf, new_reader_id);
			strcat_s(sqlbuf, "'");
 
			if (mysql_query(sock, sqlbuf))
			{
				printf("数据获取失败!:%s", mysql_error(sock));
				return;
			}
			mysql_commit(sock);
			cout << "删除成功!" << endl;
		}
	}
}
 
void reader_op(MYSQL *sock,char * reader_id)
{
	while (1)
	{
		reader_meun();
		int r_op=0;
		cin >> r_op;
 
		if (!r_op)
		{
			end_meun(sock);
		}
 
		if (r_op == 1)
		{
			char sqlbuf[200] = "SELECT * FROM book";
			if (mysql_query(sock, sqlbuf))
			{
				printf("数据获取失败!:%s", mysql_error(sock));
				return;
			}
 
			MYSQL_RES *result;
			unsigned uLine = 0, uField = 0;
			MYSQL_ROW row;
			MYSQL_FIELD *pField(NULL);
 
			if (!(result = mysql_store_result(sock)))
			{
				printf("结果获取失败!:%s", mysql_error(sock));
				return;
			}
 
			//获得行数  
			uLine = (unsigned)mysql_num_rows(result);
 
			//获得字段数  
			uField = (unsigned)mysql_num_fields(result);
 
			//填充结果集  
			cout << "书本编号          书本类型编号        书本名称        书本封面编号       书本价格         书本出版社        书本作者\n";
			for (int i = 0; i<(int)uLine; i++)
			{
				//获得一行数据  
				row = mysql_fetch_row(result);
				for (int j = 0; j<(int)uField; j++)
				{
					printf("|%-10s", row[j]);
					printf("       ");
				}
				printf("\n");
 
			}
			mysql_free_result(result);
		}
 
		else if(r_op==2)
		{
			char sqlbuf[200] = "SELECT book.book_name, borrow_situation.reader_borrow_time FROM book, borrow_situation, reader where book.book_id = borrow_situation.book_id and borrow_situation.reader_id = ";
			strcat_s(sqlbuf, reader_id);
			//cout << sqlbuf << endl;
			if (mysql_query(sock, sqlbuf))
			{
				printf("数据获取失败!:%s", mysql_error(sock));
				return;
			}
 
			MYSQL_RES *result;
			unsigned uLine = 0, uField = 0;
			MYSQL_ROW row;
			MYSQL_FIELD *pField(NULL);
 
			if (!(result = mysql_store_result(sock)))
			{
				printf("结果获取失败!:%s", mysql_error(sock));
				return;
			}
 
			//获得行数  
			uLine = (unsigned)mysql_num_rows(result);
 
			//获得字段数  
			uField = (unsigned)mysql_num_fields(result);
 
			cout << "书本名称            借阅时间           \n";
			for (int i = 0; i<(int)uLine; i++)
			{
				//获得一行数据  
				row = mysql_fetch_row(result);
				for (int j = 0; j<(int)uField; j++)
				{
					printf("|%-10s ", row[j]);
					printf("        ");
				}
				printf("\n");
 
			}
 
			mysql_free_result(result);
		}
		else if(r_op==3)
		{
			mysql_autocommit(sock, 0);
			char r_newpass[50];
			cout << "输入新的密码:";
			cin >> r_newpass;
			char sqlbuf[200] = "update reader set reader_pass=";
			strcat_s(sqlbuf, r_newpass);
			strcat_s(sqlbuf, " where reader_id = ");
			strcat_s(sqlbuf, reader_id);
			if (mysql_query(sock, sqlbuf))
			{
				printf("数据更新失败!:%s", mysql_error(sock));
				return;
			}
			mysql_commit(sock);
			mysql_autocommit(sock, 1);
			cout << "更新成功!" << endl;
		}
		else return;
	}
	
}
 
void admin(MYSQL *sock)
{
	while (1)
	{
		char admin_id[50];
		char admin_pass[50];
 
		cout << "请输入账号:";
		cin >> admin_id;
		cout << "请输入密码:";
		cin >> admin_pass;
 
		char sqlbuf[200] = "SELECT * FROM admin";
		if (mysql_query(sock, sqlbuf))
		{
			printf("数据获取失败!:%s", mysql_error(sock));
			return;
		}
 
		MYSQL_RES *result;
		unsigned uLine = 0, uField = 0;
		MYSQL_ROW row;
		MYSQL_FIELD *pField(NULL);
 
		if (!(result = mysql_store_result(sock)))
		{
			printf("结果获取失败!:%s", mysql_error(sock));
			return;
		}
 
		//获得行数  
		uLine = (unsigned)mysql_num_rows(result);
 
		//获得字段数  
		uField = (unsigned)mysql_num_fields(result);
 
		pField = mysql_fetch_fields(result);
 
		bool succ = 0;
		//填充结果集  
		for (int i = 0; i<(int)uLine; i++)
		{
			//获得一行数据  
			row = mysql_fetch_row(result);
			//cout << row[0] << " " << row[1] << endl;
			//cout << admin_id << " " << admin_pass << endl;
			//cout << strcmp((char*)row[0], admin_id) << endl;
			if (!strcmp((char*)row[0],admin_id)&& !strcmp((char*)row[1], admin_pass))
			{
				succ = 1;
				break;
			}
		}
 
		mysql_free_result(result);
 
		if (succ)
		{
			cout << "成功登陆!" << endl;
			admin_op(sock,admin_id);
			break;
		}
		else
		{
			cout << "账号或密码错误!请重新输入"<<endl;
			continue;
		}
		
	}
}
 
void reader(MYSQL *sock)
{
	while (1)
	{
		char reader_id[50];
		char reader_pass[50];
 
		cout << "请输入账号:";
		cin >> reader_id;
		cout << "请输入密码:";
		cin >> reader_pass;
 
		char sqlbuf[200] = "SELECT * FROM reader";
		if (mysql_query(sock, sqlbuf))
		{
			printf("数据获取失败!:%s", mysql_error(sock));
			return;
		}
 
		MYSQL_RES *result;
		unsigned uLine = 0, uField = 0;
		MYSQL_ROW row;
		MYSQL_FIELD *pField(NULL);
 
		if (!(result = mysql_store_result(sock)))
		{
			printf("结果获取失败!:%s", mysql_error(sock));
			return;
		}
 
		//获得行数  
		uLine = (unsigned)mysql_num_rows(result);
 
		//获得字段数  
		uField = (unsigned)mysql_num_fields(result);
 
		pField = mysql_fetch_fields(result);
 
		bool succ = 0;
		//填充结果集  
		for (int i = 0; i<(int)uLine; i++)
		{
			//获得一行数据  
			row = mysql_fetch_row(result);
			if (!strcmp((char*)row[0], reader_id) && !strcmp((char*)row[4], reader_pass))
			{
				succ = 1;
				break;
			}
		}
 
		mysql_free_result(result);
 
		if (succ)
		{
			cout << "成功登陆!" << endl;
			reader_op(sock,reader_id);
			break;
		}
		else
		{
			cout << "账号或密码错误!请重新输入" << endl;
			continue;
		}
 
	}
}
 
void main()
{
	MYSQL mysql, *sock;
	mysql_init(&mysql);
	if (!(sock = mysql_real_connect(&mysql, "localhost", "mylibrary", "****", "mylibrary", 3306, NULL, 0)))
	{
		printf("Cannot connect to database:%s\n", mysql_error(sock));
		return;
	}
	mysql_query(&mysql, "set names gbk");//输出中文
 
	mysql_autocommit(sock,0);
 
	int select_num;
	int login_num;
 
	start_meun();
 
	while (1)
	{
		cin >> select_num;
		switch (select_num)
		{
			case 1: break;
			case 0:
			{
				// 关闭数据库  
				end_meun(sock);
 
			}
			default:printf("输入错误，请重新输入:"); continue;
		}
		break;
	}
 
 
	while (1)
	{
		login_meun();
		int login_num = 0;
		cin >> login_num;
		switch (login_num)
		{
			case 1:admin(sock); break;
			case 2:reader(sock); break;
			case 0:
			{
				// 关闭数据库  
				end_meun(sock);
 
			}
			default:printf("输入错误，请重新输入:"); continue;
		}
	}
 
	mysql_close(&mysql);
 
}
 
 
 
 