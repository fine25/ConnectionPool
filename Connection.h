#pragma once
#include <mysql/mysql.h>
#include <string>
#include <ctime>
#include<thread>

using namespace std;
/*
实现mysql数据库操作
*/
class Connection
{
public:
	//初始化数据库连接池
	Connection();

	//释放数据库 连接资源
	~Connection();
	//连接数据库
	bool connect(string ip,
		unsigned short port,
		string username,
		string password,
		string dbname);
	//更新操作
	bool updata(string sql);
	//查询操作
	MYSQL_RES* query(string sql);

	//刷新一下连接的起始的空闲时间
	void refreshAliveeTime()
	{
		_alivetime = clock();
	}

	//返回存活时间
	clock_t getAliveeTime()const
	{
		return clock() - _alivetime;
	}
private:
	MYSQL* _conn; //表示h和mysql sever的一条连接
	clock_t _alivetime; //记录进入空闲状态的存活起始时间
};
