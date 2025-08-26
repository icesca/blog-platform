#ifndef MYWEBAPPSERVER_H
#define MYWEBAPPSERVER_H

#include "my_http/HttpServer.h"
#include "my_http/HttpRequest.h"
#include "my_http/HttpResponse.h"
#include "my_http/FileUtil.h"

#include "mymuduo/EventLoop.h"
#include "mymuduo/Logger.h"
#include <my_http/session/SessionManager.h>
#include <my_http/session/Session.h>
#include <my_http/JsonUtil.h>

#include <iostream>
#include <map>
#include <functional>
#include <string>
#include <memory>

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
            // 初始化中间件
            initMiddleware();
        */

        // 初始化数据库连接池
        // 不必，因为这里使用的时懒汉模式的单例模式

        // 初始化会话
        initSessionManager();

        // 初始化路由
        registerRoutes();
    }

    void registerRoutes();

    void initSessionManager();

    void setThreadNum(int num)
    {
        server_.setThreadNum(num);
    }

private:
    // # 不需要会话管理的路径：
    void testCallback(const HttpRequest &req, HttpResponse *resp);
    void faviconCallback(const HttpRequest &req, HttpResponse *resp);

    void indexCallback(const HttpRequest &req, HttpResponse *resp);
    void registerCallback(const HttpRequest &req, HttpResponse *resp);

    void list_latest_articlesCallback(const HttpRequest &req, HttpResponse *resp);
    void list_usersCallback(const HttpRequest &req, HttpResponse *resp);
    void list_user_articlesCallback(const HttpRequest &req, HttpResponse *resp);

    // # 需要会话管理的路径：
    void loginCallback(const HttpRequest &req, HttpResponse *resp);
    void logoutCallback(const HttpRequest &req, HttpResponse *resp);

    void post_articleCallback(const HttpRequest &req, HttpResponse *resp);
    void modify_articleCallback(const HttpRequest &req, HttpResponse *resp);
    void delete_articleCallback(const HttpRequest &req, HttpResponse *resp);
    void list_my_articleCallback(const HttpRequest &req, HttpResponse *resp);

    void checkLoggedinCallback(const HttpRequest &req, HttpResponse *resp);

private:
    // 这些就是进行了数据库操作的全部函数，
    // 如果要进行 SQL 语句优化，就从这里面的 SQL 语句着手
    int queryUserId(const std::string &username, const std::string &password);
    std::string queryUserData(const std::string &username);
    bool postUserData(const std::string &username, const std::string &userdata);
    int checkUserExists(const std::string &username);
    int registerUser(const std::string &username, const std::string &password);

    bool postArticle(unsigned int uid, const std::string &title, const std::string &content);
    bool modifyArticle(unsigned long long aid, const std::string &content);
    bool deleteArticle(unsigned long long aid);

    bool checkUidofAid(unsigned long long aid, unsigned int &uid);

    std::shared_ptr<json> articlesOfUser(unsigned int uid);
    std::shared_ptr<json> latestArticles(int num);
    std::shared_ptr<json> listUsers();

private:
    HttpServer server_;
    std::unique_ptr<SessionManager> session_manager_;
    std::unordered_map<int, bool> logged_in_users_;
};

#endif