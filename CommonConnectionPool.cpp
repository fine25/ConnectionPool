#include"pch.h"
#include"CommonConnectionPool.h"
#include"public.h"

using namespace std;

// 线程安全的懒汉单例函数接口
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool pool; // lock和unlock
	return &pool;
}

bool ConnectionPool::loadConfigFile()
{
	FILE* pf = fopen("mysql.ini", "r");
	if (pf == nullptr)
	{
		LOG("mysql.ini file is not exist!");
		return false;
	}

	while (!feof(pf))
	{
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		string str = line;
		int idx = str.find('=', 0);
		if (idx == -1) // 无效的配置项
		{
			continue;
		}

		// password=123456\n
		int endidx = str.find('\n', idx);
		string key = str.substr(0, idx);
		string value = str.substr(idx + 1, endidx - idx - 1);

		if (key == "ip")
		{
			_ip = value;
		}
		else if (key == "port")
		{
			_port = atoi(value.c_str());
		}
		else if (key == "username")
		{
			_username = value;
		}
		else if (key == "password")
		{
			_password = value;
		}
		else if (key == "dbname")
		{
			_dbname = value;
		}
		else if (key == "initSize")
		{
			_initSize = atoi(value.c_str());
		}
		else if (key == "maxSize")
		{
			_maxSize = atoi(value.c_str());
		}
		else if (key == "maxIdleTime")
		{
			_maxIdleTime = atoi(value.c_str());
		}
		else if (key == "connectionTimeOut")
		{
			_connectionTimeout = atoi(value.c_str());
		}
	}
	return true;
}



// 连接池的构造
ConnectionPool::ConnectionPool()
{
	// 加载配置项了
	if (!loadConfigFile())
	{
		return;
	}

	// 创建初始数量的连接
	for (int i = 0; i < _initSize; ++i)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveeTime();//刷新一下时间
		_connectionQue.push(p);
		_connectionCnt++;
	}

	// 启动一个新的线程，作为连接的生产者 linux thread => pthread_create
	thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
	produce.detach();


	// 启动一个新的定时线程，扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();
}

//生产者负责生产新线程
void ConnectionPool::produceConnectionTask()
{
	for (;;)
	{
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty())
		{
			va.wait(lock);
		}
		//创建新链接
		if (_connectionCnt < _maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveeTime();//刷新一下时间
			_connectionQue.push(p);
			_connectionCnt++;
		}
		//通知消费线程
		va.notify_all();
	}
}


//从连接池中获取一个可用的空闲连接
shared_ptr<Connection> ConnectionPool::getConnection()
{

	unique_lock<mutex> lock(_queueMutex);
	while (_connectionQue.empty())
	{
		if (cv_status::timeout == va.wait_for(lock, chrono::milliseconds(_maxIdleTime)))
		{
			if (_connectionQue.empty())
			{
				LOG("获取空闲时失败...超时了");
				return nullptr;
			}
		}
	}
	shared_ptr<Connection> sp(_connectionQue.front(),
		[&](Connection* conn) {
	unique_lock<mutex> lock(_queueMutex);
	conn->refreshAliveeTime();//刷新一下时间
	_connectionQue.push(conn);
		});
	//这段代码中定义了一个名为 sp 的变量，它的类型是 shared_ptr<Connection>。具体来说，它通过调用 _connectionQue.front() 方法获取连接队列中的第一个连接，并使用 Lambda 表达式作为自定义删除器。
	//Lambda 表达式[&](Connection* conn) { ... } 中，& 表示以引用方式捕获外部变量，这里是捕获了 _queueMutex 和 _connectionQue 两个成员变量，Connection* conn 是传入的参数，表示要删除的连接指针。Lambda 表达式的主体中，先获得 _queueMutex 的独占锁，然后将连接指针 conn 放回连接队列中，以便后续复用。
	//因此，这段代码的作用是从连接队列中取出一个连接，并创建一个共享指针 sp 来管理这个连接。当 sp 被销毁时，Lambda 表达式会被调用，将连接指针放回连接队列中，以便后续复用。这样可以实现连接的复用，减少资源浪费，提高程序效率。
	
	_connectionQue.pop();
	va.notify_all(); //通知生产者生产
	return sp;
}

void ConnectionPool::scannerConnectionTask()
{
	for (;;)
	{
		//用睡眠起到定时效果
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));
		while (_connectionCnt > _initSize)
		{
			Connection* p = _connectionQue.front();
			if (p->getAliveeTime() >= (_maxIdleTime * 1000))
			{
				_connectionQue.pop();
				_connectionCnt--;
				delete p;
			}
			else
			{
				break;
			}
		}
	}
}