# a web app is based on a http server

## it decides how to reply variou HTTP requests

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
      
       
## see

http_kama

---------------------------------
# 关于模块化的思考

- session:
HTTP 框架的用户使用它，但是一个 HTTP server 不一定需要它，
所以它作为 HTTP framework 的可选组件，在 WebApp 中使用。
如果 WebApp 的开发者需要一个 session manager, 
那么就 has a SessionManager 即可，但是一个 HttpServer 不一定需要它，一个 HttpServer 不应该 has a SessionManager.

- ConnectionPool:
类似于 SessionManager

- ThreadPool:
同理，一个 HttpServer 不需要 ThreadPool, 即便需要，它也已经利用了
TcpServer 里的 EventLoopThreadPool.
WebApp 的开发者需要 ThreadPool 的理由是可能有需要 ThreadPool 的业务逻辑 Task，那就在 WebApp 中实例化响应的 ThreadPool, 在某一个将注册到 Router 规则中的 callback 函数内，利用 ThreadPool 执行该 Task.

- TODO:
写一个需要线程池处理业务逻辑的场景的例子

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
