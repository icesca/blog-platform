# what is this
uses mymuduo + my_http library
to 
build a simple webapp.
It's actually just a onRequest() function in main.cc, 
consists of several simple routing rules.

# how it works
We have TCP server framework
HTTP server is based on TCP server
now We have HTTP server
we can set custom service based on HTTP server

# 如何实现基于 HTTP 的服务
本质上就是要实现一个
void onRequest(const HttpRequest& req, HttpResponse* resp)
函数, 并且将其注册到一个 HttpServer 的实例：
HttpServer server(&loop, InetAddress(50001), "dummy");
server.setHttpCallback(onRequest);

HTTP server 会在自己的 HttpServer::onRequest() 函数中调用这个之前被注册的 onRequest 函数

## 例子：HttpServer_test 里

的 onRequest(const HttpRequest& req, HttpResponse* resp) 函数
其实可以进一步封装，如同 HttpServer 的封装一样。

# 类比如何实现基于 TCP 的服务器

本质上就是要实现一个 
void onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp receiveTime)
函数，并且将其注册到一个 TcpServer 的实例
server_.setMessageCallback(
    std::bind(&HttpServer::onMessage, this, _1, _2, _3));

TCP server 会在自己的 TCP::onMessage() 函数中调用这个之前被注册的 onRequest 函数

## 例子：基于 TCP 实现的 HTTP 服务器：

所实现的 onMessage 函数是：
void HttpServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp receiveTime)

