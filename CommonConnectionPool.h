#pragma once
/*
ʵ�����ӳ�ģ��
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
	//��ȡ���ӳض���
	static ConnectionPool* getConnectionPool();

	//�����ӳ��л�ȡһ�����õĿ�������
	shared_ptr<Connection> getConnection();

private:
	//������ʹ���캯��˽�л�
	ConnectionPool();
	// �������ļ��м���������
	bool loadConfigFile();

	//ר�Ÿ����������߳�
	void produceConnectionTask();
	
	//ɨ���߳�,���ڻ���
	void scannerConnectionTask();

	string _ip;//mysql��IP��ַ
	unsigned short _port;//mysql�Ķ˿ں�
	string _username;   //mysql�ĵ�¼�û���
	string _password;	//mysql��¼����
	string _dbname; // ���ӵ����ݿ�����
	int _initSize;		//���ӳص�����ʼ������
	int _maxSize;		//���ӳص����������
	int _maxIdleTime;	//���ӳ�������ʱ��
	int _connectionTimeout;//���ӳػ�ȡ���ӵĳ�ʱʱ��
	
	queue<Connection*> _connectionQue; //����mysql���ӵĶӶ���
	mutex _queueMutex;  //ά�����Ӷ��е��̰߳�ȫ������
	atomic_int _connectionCnt;
	condition_variable va;
};
