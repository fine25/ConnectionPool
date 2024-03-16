#include"pch.h"
#include"CommonConnectionPool.h"
#include"public.h"

using namespace std;

// �̰߳�ȫ���������������ӿ�
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool pool; // lock��unlock
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
		if (idx == -1) // ��Ч��������
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



// ���ӳصĹ���
ConnectionPool::ConnectionPool()
{
	// ������������
	if (!loadConfigFile())
	{
		return;
	}

	// ������ʼ����������
	for (int i = 0; i < _initSize; ++i)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveeTime();//ˢ��һ��ʱ��
		_connectionQue.push(p);
		_connectionCnt++;
	}

	// ����һ���µ��̣߳���Ϊ���ӵ������� linux thread => pthread_create
	thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
	produce.detach();


	// ����һ���µĶ�ʱ�̣߳�ɨ�賬��maxIdleTimeʱ��Ŀ������ӣ����ж��ڵ����ӻ���
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();
}

//�����߸����������߳�
void ConnectionPool::produceConnectionTask()
{
	for (;;)
	{
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty())
		{
			va.wait(lock);
		}
		//����������
		if (_connectionCnt < _maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveeTime();//ˢ��һ��ʱ��
			_connectionQue.push(p);
			_connectionCnt++;
		}
		//֪ͨ�����߳�
		va.notify_all();
	}
}


//�����ӳ��л�ȡһ�����õĿ�������
shared_ptr<Connection> ConnectionPool::getConnection()
{

	unique_lock<mutex> lock(_queueMutex);
	while (_connectionQue.empty())
	{
		if (cv_status::timeout == va.wait_for(lock, chrono::milliseconds(_maxIdleTime)))
		{
			if (_connectionQue.empty())
			{
				LOG("��ȡ����ʱʧ��...��ʱ��");
				return nullptr;
			}
		}
	}
	shared_ptr<Connection> sp(_connectionQue.front(),
		[&](Connection* conn) {
	unique_lock<mutex> lock(_queueMutex);
	conn->refreshAliveeTime();//ˢ��һ��ʱ��
	_connectionQue.push(conn);
		});
	//��δ����ж�����һ����Ϊ sp �ı��������������� shared_ptr<Connection>��������˵����ͨ������ _connectionQue.front() ������ȡ���Ӷ����еĵ�һ�����ӣ���ʹ�� Lambda ���ʽ��Ϊ�Զ���ɾ������
	//Lambda ���ʽ[&](Connection* conn) { ... } �У�& ��ʾ�����÷�ʽ�����ⲿ�����������ǲ����� _queueMutex �� _connectionQue ������Ա������Connection* conn �Ǵ���Ĳ�������ʾҪɾ��������ָ�롣Lambda ���ʽ�������У��Ȼ�� _queueMutex �Ķ�ռ����Ȼ������ָ�� conn �Ż����Ӷ����У��Ա�������á�
	//��ˣ���δ���������Ǵ����Ӷ�����ȡ��һ�����ӣ�������һ������ָ�� sp ������������ӡ��� sp ������ʱ��Lambda ���ʽ�ᱻ���ã�������ָ��Ż����Ӷ����У��Ա�������á���������ʵ�����ӵĸ��ã�������Դ�˷ѣ���߳���Ч�ʡ�
	
	_connectionQue.pop();
	va.notify_all(); //֪ͨ����������
	return sp;
}

void ConnectionPool::scannerConnectionTask()
{
	for (;;)
	{
		//��˯���𵽶�ʱЧ��
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