#ifndef MYWEBAPPSERVER_H
#define MYWEBAPPSERVER_H

#include "my_http/HttpServer.h"
#include "my_http/HttpRequest.h"
#include "my_http/HttpResponse.h"
#include "my_http/FileUtil.h"

#include "mymuduo/EventLoop.h"
#include "mymuduo/Logger.h"

#include <iostream>
#include <map>
#include <functional>
#include <string>

class MyWebAppServer
{
public:
    MyWebAppServer(EventLoop *loop, const InetAddress &addr, const std::string &name) : server_(loop, addr, name)
    {
        init();
    }

    void start()
    {
        server_.start();
    }

    void init()
    {
        /*
        note:
        数据库连接池，会话管理器，中间件, FileUtil, JsonUtil
        都应该属于 HTTP 框架，而不是属于多路复用的网络框架 mymuduo.
        甚至对于 HTTP 框架的使用者来说，连 Logger 都应该是属于 HTTP 框架的，TODO: lightweight wrapper around Logger

        基于这样分层的思想，或许 TLS 模块应该在 muduo 框架之上，在 HTTP 框架之下，kama_http 的实现中的 TLS 和 HTTP 的耦合也许太大了
        */

        /* TODO:
            // 初始化数据库连接池

            // 初始化会话
            initSession();
            // 初始化中间件
            initMiddleware();
        */
        // 初始化路由
        initRouter();
    }

    void initRouter();

    void setThreadNum(int num)
    {
        server_.setThreadNum(num);
    }

private:
    void indexCallback(const HttpRequest &req, HttpResponse *resp);
    void testCallback(const HttpRequest &req, HttpResponse *resp);
    void faviconCallback(const HttpRequest &req, HttpResponse *resp);

private:
    HttpServer server_;
};

#endif