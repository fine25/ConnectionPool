# ConnectionPool

数据库连接池实现，采用生产者-消费者线程模型来设计连接的生产和连接的消费，使用了条件变量 和互斥锁来确保线程安全的通信机制。有效地提高数据库访问的性能和资源利用率，解决MySQL数据库（基于C/S设计）的访问瓶颈

此项目是在Windows平台下开发的，也可以在Linux平台下用g++也可以直接编译运行。

# 配置需求

在VS上需要进行相应MySQL的头文件和库文件的配置：

1.添加头文件路径：右键项目 - C/C++ - 常规 - 附加包含目录，填写mysql.h头文件的路径

2.添加静态库路径：右键项目 - 链接器 - 常规 - 附加库目录，填写libmysql.lib的路径

3.右键项目 - 链接器 - 输入 - 附加依赖项，填写libmysql.lib库的名字

4.添加动态库：把libmysql.dll动态链接库（Linux下后缀名是.so库）放在工程目录下

# 项目技术

基于MySQL数据库编程

基于C++11新特性编程、单例模式、queue队列容器

基于C++11多线程编程、线程互斥、线程同步通信和unique_lock

基于CAS的原子整形、智能指针shared_ptr、lambda表达式

基于生产者-消费者线程模型

# 项目功能

项目主要实现四大功能点：

初始连接量（initSize）：初始化固定的连接数量，当有连接请求时，不用再创建和MySQL Server新的连接

最大连接量（maxSize）：限定创建连接最大数量，防止连接池占用过多的socket资源，导致程序崩溃

最大空闲时间（maxIdleTime）：设定最大空闲时间，当新增加的连接都没有被再次使用过，超过该时间要自动被回收掉

连接超时时间（connectionTimeout）：当连接池中的连接数量已经到达最大，应用将通过阻塞的方式获取连接，当时间超过connectionTimeout时，那么获取连接失败，无法访问数据库

