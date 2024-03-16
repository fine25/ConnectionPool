#include"pch.h"
#include"Connection.h"
#include"CommonConnectionPool.h"
#include<iostream>
using namespace std;
int main()
{

#if 0
	//多线程 不用连接池
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
			conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//动态 IP 地址：如果你正在连接的是一个动态 IP 地址的数据库服务器，那么该 IP 地址会根据网络配置和 DHCP 服务器的分配策略而发生变化。这种情况下，你需要定期检查数据库服务器的 IP 地址，并相应地更新你的代码或配置文件。
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
			conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//动态 IP 地址：如果你正在连接的是一个动态 IP 地址的数据库服务器，那么该 IP 地址会根据网络配置和 DHCP 服务器的分配策略而发生变化。这种情况下，你需要定期检查数据库服务器的 IP 地址，并相应地更新你的代码或配置文件。
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
			conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//动态 IP 地址：如果你正在连接的是一个动态 IP 地址的数据库服务器，那么该 IP 地址会根据网络配置和 DHCP 服务器的分配策略而发生变化。这种情况下，你需要定期检查数据库服务器的 IP 地址，并相应地更新你的代码或配置文件。
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
			conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//动态 IP 地址：如果你正在连接的是一个动态 IP 地址的数据库服务器，那么该 IP 地址会根据网络配置和 DHCP 服务器的分配策略而发生变化。这种情况下，你需要定期检查数据库服务器的 IP 地址，并相应地更新你的代码或配置文件。
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
	//单线程，不用连接池
	for (int i = 0; i < 1000; i++)
	{
		Connection conn;
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 21, "male");
		conn.connect("127.0.0.1", 3306, "root", "Bb25382538", "chat");//动态 IP 地址：如果你正在连接的是一个动态 IP 地址的数据库服务器，那么该 IP 地址会根据网络配置和 DHCP 服务器的分配策略而发生变化。这种情况下，你需要定期检查数据库服务器的 IP 地址，并相应地更新你的代码或配置文件。
		conn.updata(sql);
	}
#endif // 0
#if 0
	int begin = clock();
	//单线程 使用连接池
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
	//多线程 使用连接池
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