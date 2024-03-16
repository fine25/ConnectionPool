#pragma once
#include <mysql/mysql.h>
#include <string>
#include <ctime>
#include<thread>

using namespace std;
/*
ʵ��mysql���ݿ����
*/
class Connection
{
public:
	//��ʼ�����ݿ����ӳ�
	Connection();

	//�ͷ����ݿ� ������Դ
	~Connection();
	//�������ݿ�
	bool connect(string ip,
		unsigned short port,
		string username,
		string password,
		string dbname);
	//���²���
	bool updata(string sql);
	//��ѯ����
	MYSQL_RES* query(string sql);

	//ˢ��һ�����ӵ���ʼ�Ŀ���ʱ��
	void refreshAliveeTime()
	{
		_alivetime = clock();
	}

	//���ش��ʱ��
	clock_t getAliveeTime()const
	{
		return clock() - _alivetime;
	}
private:
	MYSQL* _conn; //��ʾh��mysql sever��һ������
	clock_t _alivetime; //��¼�������״̬�Ĵ����ʼʱ��
};
