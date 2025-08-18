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
