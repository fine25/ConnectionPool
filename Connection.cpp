#include "pch.h"
#include "public.h"
#include "Connection.h"
#include <iostream>
using namespace std;

//初始化数据库连接
Connection::Connection()
{
	_conn = mysql_init(nullptr);
}

//释放数据库 连接资源
Connection::~Connection()
{
	if (_conn != nullptr)
		mysql_close(_conn);
}
//连接数据库
bool Connection::connect(string ip,unsigned short port,string username,
	string password,string dbname)
{
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), username.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);

	return p != nullptr;
}
// 更新操作 insert、delete、update
bool Connection::updata(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("更新失败:" + sql);
		std::cout << mysql_error(_conn) << std::endl;
		return false;
	}

	return true;
}
// 查询操作
MYSQL_RES* Connection::query(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("查询失败:" + sql);
		std::cout << mysql_error(_conn) << std::endl;
		return nullptr;
	}
	return mysql_use_result(_conn);
}