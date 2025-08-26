演示入口：[https://youngs.top/](https://youngs.top/)   

# 博客平台后端

## 技术要点

1. 提供 api 接口的实现  
每一个 api 接口对应一个 `void (*) (const HttpRequest& req, HttpResponse& resp)` 类型的回调函数，实现这些回调函数就是业务层后端代码的任务。  

2. 注册回调函数
使用 HTTP 框架提供的接口，将每个回调函数注册到相应的 endpoint,  
比如，将 /login 接口的回调函数 MyWebAppServer::loginCallback 注册到 POST /login 这个 endpoint:  
```
server_.Post(std::string("/login"), std::bind(&MyWebAppServer::loginCallback, this, std::placeholders::_1, std::placeholders::_2));  
```

3. 会话管理  
使用了 HTTP 框架提供的会话模块，对于每一个成功登录的用户，维护 SessionManager 和 logged_in_users_ 中的一项记录。  
```
std::unique_ptr<SessionManager> session_manager_;
std::unordered_map<int, bool> logged_in_users_;
```

4. 数据持久化  
设计了两个 MySQL 表：users, articles, 两者的记录为一对多的关系。  
根据业务中需要频繁查询的字段，增加了索引。  

## 需要实现的后台 api 接口

```
GET /
POST /login
POST /register
GET /logout

POST /post_article
POST /modify_article
POST /delete_article
GET /list_my_articles

GET /list_latest_articles
GET /list_users
POST /list_user_articles

GET /check_loggedin
```

## 使用 json 格式传输客户端和服务端之间的通信数据

比如，对于 POST /login 请求，请求和相应的数据体格式为：  

```
request body:
{
    "username": <string>,
    "password": <string>
}

OK response body:
{
    "status": "success",
    "message": <string>,
    "userId": <string>
}
```

代码中使用了 `nlohmann_json` 开源库来进行 json 数据的反序列化和序列化操作。  

## 数据库的使用

### 表设计

```sql
CREATE DATABASE blogdb;

USE blogdb;

CREATE TABLE `users` (
  `uid` INT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
  `username` VARCHAR(100) NOT NULL UNIQUE,
  `password` VARCHAR(255) NOT NULL
);

CREATE TABLE `articles` (
  `aid` BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
  `uid` INT UNSIGNED NOT NULL,
  `time` BIGINT UNSIGNED NOT NULL,
  `title` VARCHAR(200) NOT NULL,
  `content` TEXT NOT NULL
);

CREATE INDEX `idx_articles_uid` ON `articles`(`uid`);
CREATE INDEX `idx_articles_time` ON `articles`(`time`);

GRANT ALL PRIVILEGES ON blogdb.* TO testuser@localhost;
FLUSH PRIVILEGES;

```

### 数据库优化
1. 索引  
`articles(uid)`索引，因为业务中需要经常查询某一个用户发表的所有文章。  
`articles(time)`索引，因为业务中需要查询平台上最近发布的文章。  
2. 数据库连接池   
使用数据库连接池，减少频繁握手和验证的开销。  

## 前端界面使用 Claude 辅助生成

index.html 即前端文件，其中的 js 代码包含了对后端 api 的调用，其中利用了 vue3 前端框架。   

### 提示词内容

- 对期待的前端界面交互逻辑的描述，即页面视图上显示哪些内容，用户可以进行哪些操作。  
- 对后端所有 api 的描述，即每一个 endpoint 的 HTTP 请求格式，以及 HTTP 响应的格式。  

### 前后端分离的思想

提示词中对业务逻辑和 api 的描述，完全不涉及任何后端 C++ 代码的实现细节，甚至没有告知哪一种交互应该调用哪一些 api 接口来实现，描述中只是单纯地列出 api 接口的说明。同样，后端代码也不需要知道前端细节，是共同的 api 规则将二者耦合在一起。  
这样一来，实现了前后端相对独立开发的可能性，前端可以独立地用不同的前端框架重构，后端也可以用不同的语言重新实现，体现了前后端分离的思想。  

使用到的完整提示词，见 [frontend/prompt.md](frontend/prompt.md)   

----------------------
# TODO: 

- 按照 RESTful API 规范重新设计接口, see[rest](https://www.runoob.com/restfulapi/restful-api-tutorial.html), [fastapi](https://www.runoob.com/fastapi/fastapi-tutorial.html)    
- 增加用户主页信息  
- 允许用户更改个人信息   
- 评论功能  

# note

表设计的实践中，不用数据库的外键功能，把表与表之间的联系的处理交给业务代码，让数据库只做简单的 CRUD 功能。     
因为存储层容易产生性能瓶颈，实际开发中要把外键这样的关联关系放到业务代码中.  

----------------------

# api 设计的思路 (draft)

思路：先关注根据新增的数据库设计，后端需要提供什么必要的接口；再关注前端应该如何设计交互逻辑  

```
**需要增加的接口**：   

**需要验证的**（通过 session）：  

POST:   

post_article  
  req: "title", "content"
  resp: "status", "message"

modify_article
  req: "aid", "content"
  resp: "status", "message"

delete_article
  req: "aid"
  resp: "status", "message"

GET:

list_my_articles
  resp: 
  [
    {
      "aid": <string>,
      "time": <string>,
      "title": <string>,
      "content": <string>
    }, 
    ...
  ]

**不需要验证的**：

GET:

x list_all_articles
  resp: 
  [
    {
      "uid": <string>,
      "username": <string>,
      "articles": [
        {
          "time": <string>,
          "title": <string>,
          "content": <string>
        },
        ...
      ]
    },
    {
      "uid": <string>,
      "username": <string>,
      "articles": [
        ...
      ]
    }
  ]

list_latest_articles
  resp:
  [
    {
      "aid": <string>,
      "time": <string>,
      "title": <string>,
      "content": <string>,
      "uid": <string>,
      "username": <string>
    }
  ]

list_users
  resp:
  [
    {
      "uid": <string>,
      "username": <string>,
      "num_of_articles": <string>
    },
    ...
  ]

POST:
list_user_articles
  req: "uid"
  resp: 
  [
    {
      "aid": <string>,
      "time": <string>,
      "title": <string>,
      "content": <string>
    }, 
    ...
  ]
```

------------------------------------
# 感谢以下开源项目

https://github.com/nlohmann/json
