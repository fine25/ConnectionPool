#pragma once
/*
实现连接池模块
*/
#include<condition_variable>
#include<memory>
#include "Connection.h"
#include<queue>
#include<mutex>
#include<atomic>
#include <iostream>
#include<thread>
#include<chrono>
#include<functional>
class ConnectionPool
{
public:
	//获取连接池对象
	static ConnectionPool* getConnectionPool();

	//从连接池中获取一个可用的空闲连接
	shared_ptr<Connection> getConnection();

private:
	//单例，使构造函数私有化
	ConnectionPool();
	// 从配置文件中加载配置项
	bool loadConfigFile();

	//专门负责生产新线程
	void produceConnectionTask();
	
	//扫描线程,用于回收
	void scannerConnectionTask();

	string _ip;//mysql的IP地址
	unsigned short _port;//mysql的端口号
	string _username;   //mysql的登录用户名
	string _password;	//mysql登录密码
	string _dbname; // 连接的数据库名称
	int _initSize;		//连接池的最大初始连接量
	int _maxSize;		//连接池的最大连接量
	int _maxIdleTime;	//连接池最大空闲时间
	int _connectionTimeout;//连接池获取连接的超时时间
	
	queue<Connection*> _connectionQue; //储存mysql连接的队队列
	mutex _queueMutex;  //维护链接队列的线程安全互斥锁
	atomic_int _connectionCnt;
	condition_variable va;
};
