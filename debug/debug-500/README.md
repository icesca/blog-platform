- branch: debug-500

# 描述与复现

- bug 描述
  - 运行一天后，/list_users, /list_latest_articles 接口有很高的返回 500 的频率，之后完全变成了只有 500 回复

- 如何复现：
- 猜想：请求次数有关
- 复现 bug
  - 使用 libcpr 模拟大量请求
  - 在另一台机器运行 get_test, 不要在同一台服务器上运行
- 复现失败：
  get_test 大量请求都能成功返回，可以判断该 bug 与请求次数无关
  get_test 返回 500 只出现在运行一天之后

- 难点：难以复现，只能等经过一天的时间后才能得到 500 状态


TODO:
- 猜想：什么与时间有关？
 - 客户端 conn_pool 中连接的超时时间？
 - MySQL 服务器端维护的连接的超时时间？
   - Yes! see # 成功复现
- 查看 mysql_query() 返回 0 的可能情况
  - mysql_error()

# 成功复现：

SET GLOBAL wait_timeout = 60;  
SET GLOBAL interactive_timeout = 60;
然后查看：
show variables like '%connect%';
or
show status like 'Threads_connected';
or
show processlist;

# debug

- 复现之后（或者仅仅是等待出现 500 之后）

- 代码溯源：

一定是 数据库操作函数 listUsers() 返回了 nullptr, 其返回 nullptr 有两种可能性，
查看日志，判断一定是 conn_sp->query() 返回了 nullptr,
继续代码溯源 + 查看日志，判断一定是官方 mysql_query() 函数返回了非 0

使用命令行手动连接数据库并进行相同的查询，没有问题，判断数据库服务器端没有问题，
SELECT users.uid,username,COUNT(aid) num_of_articles FROM articles LEFT JOIN users ON articles.uid = users.uid GROUP BY uid

get_test.cc  

```cpp
#include <iostream>
#include <fstream>
#include <chrono>

#include <cpr/cpr.h>

int main()
{
        std::ofstream of("get_text.txt");

        int times = 10000;

        cpr::Response resp;

        for (int i = 0; i < times; ++i)
        {

                resp = cpr::Get(cpr::Url{"http://116.62.33.139:50000/list_users"});
                of << resp.status_code << std::endl;

                resp = cpr::Get(cpr::Url{"http://116.62.33.139:50000/list_latest_articles"});
                of << resp.status_code << std::endl;

                std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
}
```

# TO FIX

应该在 ConnectionPool 模块中修复，该问题并不是应用层的 bug
