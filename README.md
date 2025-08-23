# a web app is based on a http server

## it decides how to reply various HTTP requests

- set onRequest of the http lib to decide what to reply when get requests of
  - GET /<url> ...
  - POST /<url> ...
  - HEAD /<url> ...
- by composing a Request object

## what to do

- index.html
  - get
    - login.html
      - post
        - passwd correct
          - user_page.html
        - incorrect
          - login.html
    - signup.html
      - post
        - no duplicate
          - user_page.html
        - duplicate
          - signup.html
      

------------------------
# 表设计

CREATE DATABASE testdb;

CREATE TABLE `users` (
  `id` int PRIMARY KEY AUTO_INCREMENT,
  `username` varchar(100) NOT NULL UNIQUE,
  `password` varchar(100) NOT NULL,
  `userdata` text
);

GRANT ALL PRIVILEGES ON testdb.* TO testuser@localhost;
FLUSH PRIVILEGES;

------------------------
# 更新日志

## update: （用户表和博客表分离，这样支持为每个用户保存多条博客）

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

思路：先关注根据新增的数据库设计，后端需要提供什么必要的接口；再关注前端应该如果设计交互逻辑

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

list_hot_articles
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



  

# TODO: 

重新生成 vue 框架的前端
按照 RESTful API 规范重新设计接口, see[rest](https://www.runoob.com/restfulapi/restful-api-tutorial.html), [fastapi](https://www.runoob.com/fastapi/fastapi-tutorial.html)  
增加用户主页信息
允许用户更改个人信息
评论功能

# note

表设计的实践中，外键不用？
因为存储层容易产生性能瓶颈，实际开发中要把外键这样的关联关系放到业务代码中
