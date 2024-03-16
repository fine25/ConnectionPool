# ConnectionPool
数据库连接池实现，采用生产者-消费者线程模型来设计连接的生产和连接的消费，使用了条件变量 和互斥锁来确保线程安全的通信机制


# 项目技术
基于MySQL数据库编程、
基于C++11新特性编程、单例模式、queue队列容器、
基于C++11多线程编程、线程互斥、线程同步通信和unique_lock
基于CAS的原子整形、智能指针shared_ptr、lambda表达式
基于生产者-消费者线程模型

# 项目功能
项目主要实现四大功能点：
初始连接量（initSize）
最大连接量（maxSize）
最大空闲时间（maxIdleTime）
连接超时时间（connectionTimeout）
