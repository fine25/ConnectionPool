#include "pch.h"
#include "public.h"
#include "Connection.h"
#include <iostream>
using namespace std;

//��ʼ�����ݿ�����
Connection::Connection()
{
	_conn = mysql_init(nullptr);
}

//�ͷ����ݿ� ������Դ
Connection::~Connection()
{
	if (_conn != nullptr)
		mysql_close(_conn);
}
//�������ݿ�
bool Connection::connect(string ip,unsigned short port,string username,
	string password,string dbname)
{
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), username.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);

	return p != nullptr;
}
// ���²��� insert��delete��update
bool Connection::updata(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("����ʧ��:" + sql);
		std::cout << mysql_error(_conn) << std::endl;
		return false;
	}

	return true;
}
// ��ѯ����
MYSQL_RES* Connection::query(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("��ѯʧ��:" + sql);
		std::cout << mysql_error(_conn) << std::endl;
		return nullptr;
	}
	return mysql_use_result(_conn);
}