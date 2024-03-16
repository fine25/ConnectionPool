#include"pch.h"
#include"Connection.h"
#include"CommonConnectionPool.h"
#include<iostream>
using namespace std;
int main()
{

#if 0
	//���߳� �������ӳ�
	Connection conn;
	conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");
	clock_t begin = clock();
	thread t1([]() {
		for (int i = 0; i < 250; i++)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 21, "male");
			conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//��̬ IP ��ַ��������������ӵ���һ����̬ IP ��ַ�����ݿ����������ô�� IP ��ַ������������ú� DHCP �������ķ�����Զ������仯����������£�����Ҫ���ڼ�����ݿ�������� IP ��ַ������Ӧ�ظ�����Ĵ���������ļ���
			conn.updata(sql);
		};
		});
	thread t2([]() {
		for (int i = 0; i < 250; i++)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 21, "male");
			conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//��̬ IP ��ַ��������������ӵ���һ����̬ IP ��ַ�����ݿ����������ô�� IP ��ַ������������ú� DHCP �������ķ�����Զ������仯����������£�����Ҫ���ڼ�����ݿ�������� IP ��ַ������Ӧ�ظ�����Ĵ���������ļ���
			conn.updata(sql);
		};
		});
	thread t3([]() {
		for (int i = 0; i < 250; i++)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 21, "male");
			conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//��̬ IP ��ַ��������������ӵ���һ����̬ IP ��ַ�����ݿ����������ô�� IP ��ַ������������ú� DHCP �������ķ�����Զ������仯����������£�����Ҫ���ڼ�����ݿ�������� IP ��ַ������Ӧ�ظ�����Ĵ���������ļ���
			conn.updata(sql);
		};
		});
	thread t4([]() {
		for (int i = 0; i < 250; i++)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 21, "male");
			conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//��̬ IP ��ַ��������������ӵ���һ����̬ IP ��ַ�����ݿ����������ô�� IP ��ַ������������ú� DHCP �������ķ�����Զ������仯����������£�����Ҫ���ڼ�����ݿ�������� IP ��ַ������Ӧ�ظ�����Ĵ���������ļ���
			conn.updata(sql);
		};
		});
	t1.join();
	t2.join();
	t3.join();
	t4.join();
#endif


#if 1
	int begin = clock();
	//���̣߳��������ӳ�
	for (int i = 0; i < 1000; i++)
	{
		Connection conn;
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 21, "male");
		conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//��̬ IP ��ַ��������������ӵ���һ����̬ IP ��ַ�����ݿ����������ô�� IP ��ַ������������ú� DHCP �������ķ�����Զ������仯����������£�����Ҫ���ڼ�����ݿ�������� IP ��ַ������Ӧ�ظ�����Ĵ���������ļ���
		conn.updata(sql);
	}
#endif // 0
#if 0
	int begin = clock();
	//���߳� ʹ�����ӳ�
	ConnectionPool* cp = ConnectionPool::getConnectionPool();

	for (int i = 0; i < 1000; i++)
	{
		shared_ptr<Connection> sp = cp->getConnection();
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 21, "male");
		sp->updata(sql);

	}
#endif // 0


#if 0
	//���߳� ʹ�����ӳ�
	clock_t begin = clock();
	thread t1([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
	for (int i = 0; i < 250; i++)
	{
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 21, "male");
		shared_ptr<Connection> sp = cp->getConnection();
		sp->updata(sql);
	};
		});
	thread t2([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
	for (int i = 0; i < 250; i++)
	{
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 21, "male");
		shared_ptr<Connection> sp = cp->getConnection();
		sp->updata(sql);
	};
		});
	thread t3([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
	for (int i = 0; i < 250; i++)
	{
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 21, "male");
		shared_ptr<Connection> sp = cp->getConnection();
		sp->updata(sql);
	};
		});
	thread t4([]() {
		ConnectionPool* cp = ConnectionPool::getConnectionPool();
	for (int i = 0; i < 250; i++)
	{
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 21, "male");
		shared_ptr<Connection> sp = cp->getConnection();
		sp->updata(sql);
	};
		});
	t1.join();
	t2.join();
	t3.join();
	t4.join();
#endif

	clock_t end = clock();
	cout << end - begin<<"ms" << endl;
	return 0;
}