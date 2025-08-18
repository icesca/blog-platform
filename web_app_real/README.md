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

---------------------------
# 职责分离的考虑

## 什么由 HttpServer 模块来处理
以处理 400 Bad Request 为例，

HttpRequest 的违反协议（语法）的格式错误应该由 HttpServer 模块来处理,
应该直接由 HttpServer 给客户端响应，不用传递给 WebApp 模块处理。
比如：
POST 但是没有 Content-Type 字段 or Content-Length 字段

## 什么由 WebApp 模块来处理
WebApp 收到 HttpRequest 后，也可能发送 400 Bad Request 错误，

HttpRequest 到达 WebApp 时，已经被 HttpServer 保证没有 缺失 Content-Type, Content-Length, 数据长度不对 这样的错误，

WebApp 仅处理业务逻辑中出现的格式错误（对于HTTP协议来说是语义错误，对于应用层来说是格式错误），
比如：
业务逻辑需要 application/json 类型的数据，而接收到的 Content-Type != "application/json";
json 数据的格式解析错误;

------------------------------------
# 关于 loginCallback 的登陆逻辑

一个会话何时开始，何时结束，应该由 WebApp 的应用逻辑决定：

## 起初我认为 http_kama 中的 WebApp 是窗口型的应用：

- 一个窗口的打开开启一个会话，同一个窗口再次关闭时该会话也同时关闭。
- 没有登陆的窗口也有一个会话 id 
- 可以有多个窗口同时打开，登录不同的账号密码，每个窗口会被分配自己的 sessionId

## 后来发现上面的逻辑无法实现，因为对于同一个站点，浏览器只保存了同一个 sessionId

- see main branch, 两个窗口登录不同的账号名密码，在 Web Developer Tools 中可以看到，两个同时登录的不同用户的 sessionId 居然是一样的, 因为两个窗口共享相同的 Storage
- 除非是在两个 Cooike 隔离的环境中, 比如一个 Private window

----------------------------
# debug log

## 已修复
- mt19937() 处 segment fault
  - 因为没有初始化 MyWebAppServer 里的 session_manager_ 里的 storage_

----------------------------
# TODO: bug to fix

- 可能是因为手机浏览器退出窗口后不保存 cookie? 在手机端登录后，退出浏览器，再次进入，会出现登录状态丢失的情况。
  这样就会出现该登录状态在服务端有记录，但客户端因为丢失了 cookie 而无法获取到登录状态。
 
  解决方案 todo:
  在会话管理的逻辑处处理：如果再次登录的账户密码正确，即使 login request 中没有 cookie, 也返回已有的 session 的 cookie 作为登录响应。

# TODO: feature to add

- 增加定时器关闭超时的 session，减少服务器资源占用 （storage_.load() 并不能完全处理，因为它只有在通过确切的 sessionId 访问时才会清除过期 session）
  这实际上是 HTTP 框架的任务

# TODO: improve

- play with CMakeLists.txt
  (see Claude history)
