#include "MyWebAppServer.h"
#include <my_http/conn_pool/ConnectionPool.h>
#include <my_http/JsonUtil.h>
#include <memory>
#include "favicon.h"
#include <regex>

void MyWebAppServer::registerRoutes()
{
    // Q: why this doesn't work
    // server_.Get(std::string("/"), std::bind(MyWebAppServer::entryCallback, this, std::placeholders::_1, std::placeholders::_2));

    server_.Get(std::string("/test"), std::bind(&MyWebAppServer::testCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Get(std::string("/favicon.ico"), std::bind(&MyWebAppServer::faviconCallback, this, std::placeholders::_1, std::placeholders::_2));

    server_.Get(std::string("/"), std::bind(&MyWebAppServer::indexCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Post(std::string("/login"), std::bind(&MyWebAppServer::loginCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Post(std::string("/register"), std::bind(&MyWebAppServer::registerCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Get(std::string("/logout"), std::bind(&MyWebAppServer::logoutCallback, this, std::placeholders::_1, std::placeholders::_2));

    server_.Post(std::string("/post_article"), std::bind(&MyWebAppServer::post_articleCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Post(std::string("/modify_article"), std::bind(&MyWebAppServer::modify_articleCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Post(std::string("/delete_article"), std::bind(&MyWebAppServer::delete_articleCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Get(std::string("/list_my_articles"), std::bind(&MyWebAppServer::list_my_articleCallback, this, std::placeholders::_1, std::placeholders::_2));

    server_.Post(std::string("/list_user_articles"), std::bind(&MyWebAppServer::list_user_articlesCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Get(std::string("/list_latest_articles"), std::bind(&MyWebAppServer::list_latest_articlesCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Get(std::string("/list_users"), std::bind(&MyWebAppServer::list_usersCallback, this, std::placeholders::_1, std::placeholders::_2));

    server_.Get(std::string("/check_loggedin"), std::bind(&MyWebAppServer::checkLoggedinCallback, this, std::placeholders::_1, std::placeholders::_2));
}

void MyWebAppServer::initSessionManager()
{
    session_manager_ = std::make_unique<SessionManager>(std::make_unique<MemorySessionStorage>());
}

// # 不需要会话管理的路径：
// GET
void MyWebAppServer::testCallback(const HttpRequest &req, HttpResponse *resp)
{
    resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
    resp->setContentType("text/html");
    resp->addHeader("Server", "fake server name");
    std::string now = Timestamp::now().toString();
    std::string body =
        "<!DOCTYPE html>"
        "<html><head><title>This is title</title></head>"
        "<body><h1>Hello</h1>Now is " +
        now +
        "</body></html>";
    resp->setBody(body);
    resp->setContentLength(body.size());
}

// GET
void MyWebAppServer::faviconCallback(const HttpRequest &req, HttpResponse *resp)
{
    resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
    resp->setContentType("image/png");
    resp->setBody(favicon);
    LOG_TRACE("sizeof favicon: %lu", favicon.size());
    resp->setContentLength(favicon.size());
}

// GET
void MyWebAppServer::indexCallback(const HttpRequest &req, HttpResponse *resp)
{
    std::string req_pathname;
    req_pathname.append("../frontend/index.html");
    FileUtil index_file(req_pathname);
    if (!index_file.isValid())
    {
        LOG_FATAL("MyWebAppServer::%s() => %s does not exist, aborting", __func__, req_pathname.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(1));
        abort();
    }

    // Q: why std::vector<char> then std::string
    std::vector<char> buffer(index_file.size());
    index_file.readFile(buffer); // 读出文件数据
    std::string bufStr = std::string(buffer.data(), buffer.size());

    resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
    resp->setCloseConnection(false);
    resp->setContentType("text/html");
    resp->setContentLength(bufStr.size());
    /*
    不用担心 bufStr 传入 setBody() 后会被回收，因为：
    void setBody(const std::string& body)  // 这里是引用
    {
        body_ = body;   // 但是这里不是
        // TODO: 用 move 语义如何?
    }
    */
    resp->setBody(bufStr);
}

// POST
void MyWebAppServer::registerCallback(const HttpRequest &req, HttpResponse *resp)
{
    /*
    # 可能的回应：
    400 Bad Request
      格式错误
    403 Forbidden
      用户名或密码不合法（已有相同的用户名或用户名密码超长）
    200 OK
      注册成功
    */

    // 1. 检查 400 Bad Request 错误
    std::string content_type = req.getHeader("Content-Type");
    std::string body = req.getBody();

    // body.empty() 的问题在 HttpContext::parseRequest() 中已经反馈了
    if (content_type != "application/json" || body.empty())
    {
        LOG_INFO("get content:\n%s", body.c_str());

        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "格式错误";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    try
    {
        json parsed = json::parse(req.getBody());
        std::string username = parsed["username"];
        std::string password = parsed["password"];

        // # 2. 检查 403 Forbidden 错误
        int userId = checkUserExists(username);

        if (userId != -1)
        {
            // 封装json数据
            json failureResp;
            failureResp["status"] = "fail";
            failureResp["message"] = "用户名已存在，或用户名密码超长";
            std::string failureBody = failureResp.dump(4);

            resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
            resp->setCloseConnection(true);
            resp->setContentType("application/json");
            resp->setContentLength(failureBody.size());
            resp->setBody(failureBody);
            return;
        }

        // # 3. 执行到这里，才终于 200 OK
        // getSession(req, resp) 的接口就像一个中间件一样
        // auto session = session_manager_->getSession(req, resp);

        // 真正重要的业务逻辑就这些
        int res_id = registerUser(username, password);
        ///

        json successResp;
        successResp["status"] = "success";
        successResp["message"] = "registered new account";
        successResp["userId"] = res_id;
        std::string successBody = successResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
        return;
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        // 可能是 json::parse() 失败
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}

// GET
void MyWebAppServer::list_latest_articlesCallback(const HttpRequest &req, HttpResponse *resp)
{
    std::shared_ptr<json> j = latestArticles(5);
    if (j == nullptr)
    {
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "failed getting latest articles";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k500InternalServerError, "InternalServerError");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
    else
    {
        json successResp;
        successResp["status"] = "success";
        successResp["message"] = "got latest articles";
        successResp["articles"] = *j;
        std::string successBody = successResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "Ok");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
        return;
    }
}

// GET
void MyWebAppServer::list_usersCallback(const HttpRequest &req, HttpResponse *resp)
{
    std::shared_ptr<json> j = listUsers();
    if (j == nullptr)
    {
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "failed getting user list";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k500InternalServerError, "InternalServerError");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
    else
    {
        json successResp;
        successResp["status"] = "success";
        successResp["message"] = "got user list";
        successResp["users"] = *j;
        std::string successBody = successResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "Ok");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
        return;
    }
}

// POST
void MyWebAppServer::list_user_articlesCallback(const HttpRequest &req, HttpResponse *resp)
{
    /*
    # 可能的回应：
    400 Bad Request
      格式错误
    403 Forbidden
      用户名或密码不合法（已有相同的用户名或用户名密码超长）
    200 OK
      注册成功
    */

    // 1. 检查 400 Bad Request 错误
    std::string content_type = req.getHeader("Content-Type");
    std::string body = req.getBody();

    // body.empty() 的问题在 HttpContext::parseRequest() 中已经反馈了
    if (content_type != "application/json" || body.empty())
    {
        LOG_INFO("get content:\n%s", body.c_str());

        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "格式错误";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    try
    {
        json parsed = json::parse(req.getBody());
        unsigned int uid = std::stoi(parsed["uid"].get<std::string>());

        // 真正重要的业务逻辑就这些
        std::shared_ptr<json> j = articlesOfUser(uid);
        ///

        if (j == nullptr)
        {
            json failureResp;
            failureResp["status"] = "fail";
            failureResp["message"] = "failed getting articles";
            std::string failureBody = failureResp.dump(4);

            resp->setStatusLine(req.getVersionStr(), HttpResponse::k500InternalServerError, "InternalServerError");
            resp->setCloseConnection(true);
            resp->setContentType("application/json");
            resp->setContentLength(failureBody.size());
            resp->setBody(failureBody);
            return;
        }
        else
        {
            json successResp;
            successResp["status"] = "success";
            successResp["message"] = "got articles";
            successResp["articles"] = *j;
            std::string successBody = successResp.dump(4);

            resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "Ok");
            resp->setCloseConnection(true);
            resp->setContentType("application/json");
            resp->setContentLength(successBody.size());
            resp->setBody(successBody);
            return;
        }
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        // 可能是 json::parse() 失败
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}

// # 需要会话管理的路径：

// POST
void MyWebAppServer::loginCallback(const HttpRequest &req, HttpResponse *resp)
{
    // kama_http 中 LoginHandler 的逻辑很混乱
    // 因为判断用户名密码正确后，应该检查是否已登录...

    /*
    # 可能的回应：
    400 Bad Request
      格式错误
      处理 login, 那么就必然要先从 request body 中获取账号密码
    403 Forbidden
      - 该请求中带有有效的sessionId(能够在session_storage中找到并且是loggin状态)
        也就是说客户端已经有了一个处于连接状态的会话
      - 如果用户请求没有携带有效的 sessionId，但是携带了有效的 username 和 password，
        但是该用户已经有一个没有退出的会话。
        TODO: 这里的业务逻辑是：一个用户只能有一个登录会话. 那么这样就要考虑需要设置一个定时器，
              对长期没要操作的会话清除会话信息，客户端会话被遗忘一直被 logout 的情况下，在其他设备无法再登录该用户。
    401 Unauthorized
      在查找数据库后，发现账号密码不合法
    200 OK
      成功登录并创建会话，客户端会收到有效的 sessionId
    */

    // 1. 检查 400 Bad Request 错误

    std::string content_type = req.getHeader("Content-Type");
    std::string body = req.getBody();

    // body.empty() 的问题在 HttpContext::parseRequest() 中已经反馈了
    if (content_type != "application/json" || body.empty())
    {
        LOG_INFO("%s() => 400 Bad Request: get content:\n%s", __func__, body.c_str());

        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "格式错误";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    // # 2. 检查 403 Forbidden 错误

    std::shared_ptr<Session> se = session_manager_->getSession(req);
    if (se)
    {
        if (se->getValue("isLoggedIn") == "true")
        {
            json failureResp;
            failureResp["status"] = "fail";
            failureResp["message"] = "当前已有账号登录，请先退出";
            std::string failureBody = failureResp.dump(4);

            LOG_INFO("get content:\n%s", body.c_str());
            resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
            resp->setCloseConnection(true);
            resp->setContentType("application/json");
            resp->setContentLength(failureBody.size());
            resp->setBody(failureBody);
            return;
        }
    }

    try
    {
        // 格式没有问题了，那么就 从 json 中提取用户名密码，
        // 在数据库中验证用户名密码的正确性
        json parsed = json::parse(req.getBody());
        std::string username = parsed["username"];
        std::string password = parsed["password"];

        int userId = queryUserId(username, password);

        // # 3. 检查 401 Unauthorized 错误

        if (userId == -1)
        {
            // # 3.1 账号密码错误

            json failureResp;
            failureResp["status"] = "fail";
            failureResp["message"] = "Invalid username or password";
            std::string failureBody = failureResp.dump(4);

            resp->setStatusLine(req.getVersionStr(), HttpResponse::k401Unauthorized, "Unauthorized");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(failureBody.size());
            resp->setBody(failureBody);
            return;
        }
        else
        {
            // # 3.2 账号密码正确

            // 再次检查 403 Forbidden 错误,

            // 如果用户请求没有携带有效的 sessionId，但是携带了有效的 username 和 password，
            // 那么就检查该用户是否已经在别处登录，也就是是否服务端有该用户对应的 session 记录,
            // 避免用户重复生成 sessionId.
            // 不好直接用 SessionManager 查看是否已登录, 因为 SessionStorage 维护的是 SessionId --> Session 的映射，
            // 不方便以 userId or username 检索，
            // 所以 WebApp 这里要维护一个已经登陆的 users 的 uid 的列表,
            if (logged_in_users_.count(userId) != 0 && logged_in_users_[userId] == true)
            {
                json failureResp;
                failureResp["status"] = "fail";
                failureResp["message"] = "该用户已在别处登录，请先登出";
                std::string failureBody = failureResp.dump(4);

                LOG_INFO("get content:\n%s", body.c_str());
                resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
                resp->setCloseConnection(true);
                resp->setContentType("application/json");
                resp->setContentLength(failureBody.size());
                resp->setBody(failureBody);
                return;
            }

            // # 4. 执行到这里，才终于 200 OK
            // getSession(req, resp) 的接口就像一个中间件一样
            // auto session = session_manager_->getSession(req, resp);

            // 真正重要的业务逻辑就这些
            auto session = session_manager_->setSessionToResp(resp);
            session->setValue("userId", std::to_string(userId));
            session->setValue("username", username);
            session->setValue("isLoggedIn", "true");
            logged_in_users_[userId] = true; // 维护一个已登录用户的列表
            ///

            json successResp;
            successResp["status"] = "success";
            successResp["userId"] = userId;
            std::string successBody = successResp.dump(4);

            resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(successBody.size());
            resp->setBody(successBody);
            return;
        }
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        // 可能是 json::parse() 失败
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}

// GET
void MyWebAppServer::logoutCallback(const HttpRequest &req, HttpResponse *resp)
{
    /*
    完全可以基于 loginCallback() 做出简单的修改，主要的区别是 200 时的逻辑

    # 可能的回应：
    403 Forbidden
      头部没有有效的 sessionId, 说明根本就没有登录，更无从谈 logout
    200 OK
      成功退出登录，客户端会收到有效的 sessionId
    */

    // # 检查 403 Forbidden 错误
    std::shared_ptr<Session> se = session_manager_->getSession(req);
    if (!se || se->getValue("isLoggedIn") != "true")
    {
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "当前没有已登录的账号";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    try
    {
        // # 执行到这里，没有问题，说明 se && isLoggedIn
        //   200 OK

        // 真正执行的任务其实只有这一行
        session_manager_->destroySession(se);
        logged_in_users_[std::stoi(se->getValue("userId"))] = false; // 维护一个已登录用户的列表
        ///

        json response;
        response["status"] = "success";
        response["message"] = "logout success";
        std::string responseBody = response.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(responseBody.size());
        resp->setBody(responseBody);
        return;
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}

// POST
void MyWebAppServer::post_articleCallback(const HttpRequest &req, HttpResponse *resp)
{
    /*
    # 可能的回应：
    400 Bad Request
      格式错误
    403 Forbidden
      req 中不带有有效的 sessionId
    200 OK
      成功获取到 userdata, 通过 json 返回
    */

    // # 检查 400 Bad Request 错误
    std::string content_type = req.getHeader("Content-Type");
    std::string body = req.getBody();

    if (content_type != "application/json" || body.empty())
    {
        LOG_INFO("get content:\n%s", body.c_str());

        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "格式错误";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    // # 检查 403 Forbidden 错误
    std::shared_ptr<Session> se = session_manager_->getSession(req);
    if (!se || se->getValue("isLoggedIn") != "true")
    {
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "当前没有已登录的账号";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    try
    {
        // # 执行到这里，才终于 200 OK
        json parsed = json::parse(req.getBody());
        int uid = std::stoi(se->getValue("userId"));
        std::string title = parsed["title"];
        std::string content = parsed["content"];
        // 真正重要的业务逻辑就这些
        bool res = postArticle(uid, title, content);
        ///

        json successResp;
        if (res)
        {
            successResp["status"] = "success";
            successResp["mesage"] = "posted article";
            resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
        }
        else
        {
            successResp["status"] = "fail";
            successResp["mesage"] = "failed";
            resp->setStatusLine(req.getVersionStr(), HttpResponse::k500InternalServerError, "Internal Server Error");
        }

        std::string successBody = successResp.dump(4);
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
        return;
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}

// POST
void MyWebAppServer::modify_articleCallback(const HttpRequest &req, HttpResponse *resp)
{
    /*
    # 可能的回应：
    400 Bad Request
      格式错误
    403 Forbidden
      req 中不带有有效的 sessionId
    200 OK
      成功获取到 userdata, 通过 json 返回
    */

    // # 检查 400 Bad Request 错误
    std::string content_type = req.getHeader("Content-Type");
    std::string body = req.getBody();

    if (content_type != "application/json" || body.empty())
    {
        LOG_INFO("get content:\n%s", body.c_str());

        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "格式错误";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    // # 检查 403 Forbidden 错误
    std::shared_ptr<Session> se = session_manager_->getSession(req);
    if (!se || se->getValue("isLoggedIn") != "true")
    {
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "当前没有已登录的账号";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    try
    {
        // # 执行到这里，才终于 200 OK
        bool res = true;

        json parsed = json::parse(req.getBody());
        unsigned int uid = std::stoi(se->getValue("userId"));
        unsigned long long aid = std::stoll(parsed["aid"].get<std::string>());
        unsigned int uid_of_aid;
        if (!checkUidofAid(aid, uid_of_aid))
        {
            res = false;
        }
        else
        {
            if (uid != uid_of_aid)
                res = false;
        }

        if (res == true)
        {
            std::string content = parsed["content"];
            // 真正重要的业务逻辑就这些
            res = modifyArticle(aid, content);
            ///
        }

        json successResp;
        if (res)
        {
            successResp["status"] = "success";
            successResp["mesage"] = "modified article";
            resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
        }
        else
        {
            successResp["status"] = "fail";
            successResp["mesage"] = "failed";
            resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
        }

        std::string successBody = successResp.dump(4);
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
        return;
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}

// POST
void MyWebAppServer::delete_articleCallback(const HttpRequest &req, HttpResponse *resp)
{
    /*
    # 可能的回应：
    400 Bad Request
      格式错误
    403 Forbidden
      req 中不带有有效的 sessionId
    200 OK
      成功获取到 userdata, 通过 json 返回
    */

    // # 检查 400 Bad Request 错误
    std::string content_type = req.getHeader("Content-Type");
    std::string body = req.getBody();

    if (content_type != "application/json" || body.empty())
    {
        LOG_INFO("get content:\n%s", body.c_str());

        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "格式错误";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    // # 检查 403 Forbidden 错误
    std::shared_ptr<Session> se = session_manager_->getSession(req);
    if (!se || se->getValue("isLoggedIn") != "true")
    {
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "当前没有已登录的账号";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    try
    {
        // # 执行到这里，才终于 200 OK
        bool res = true;

        json parsed = json::parse(req.getBody());
        unsigned int uid = std::stoi(se->getValue("userId"));
        unsigned long long aid = std::stoll(parsed["aid"].get<std::string>());
        unsigned int uid_of_aid;
        if (!checkUidofAid(aid, uid_of_aid))
        {
            res = false;
        }
        else
        {
            if (uid != uid_of_aid)
                res = false;
        }

        if (res == true)
        {
            res = deleteArticle(aid);
            ///
        }

        json successResp;
        if (res)
        {
            successResp["status"] = "success";
            successResp["mesage"] = "deleted article";
            resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
        }
        else
        {
            successResp["status"] = "fail";
            successResp["mesage"] = "failed";
            resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
        }
        std::string successBody = successResp.dump(4);

        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
        return;
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}

// GET
void MyWebAppServer::list_my_articleCallback(const HttpRequest &req, HttpResponse *resp)
{

    /*
    # 可能的回应：
    403 Forbidden
      req 中不带有有效的 sessionId
    200 OK
      成功获取到 userdata, 通过 json 返回
    */

    // # 检查 403 Forbidden 错误
    std::shared_ptr<Session> se = session_manager_->getSession(req);
    if (!se || se->getValue("isLoggedIn") != "true")
    {
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "当前没有已登录的账号";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    try
    {
        // # 执行到这里，才终于 200 OK
        unsigned int uid = std::stoi(se->getValue("userId"));
        std::shared_ptr<json> res = articlesOfUser(uid);
        json successResp;

        if (res)
        {
            successResp["status"] = "success";
            successResp["mesage"] = "listed articles";
            successResp["articles"] = *res;
            resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
        }
        else
        {
            successResp["status"] = "fail";
            successResp["mesage"] = "failed get articles";
            resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
        }

        std::string successBody = successResp.dump(4);
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
        return;
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k500InternalServerError, "Internal Server Error");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}

// GET
void MyWebAppServer::checkLoggedinCallback(const HttpRequest &req, HttpResponse *resp)
{
    std::shared_ptr<Session> se = session_manager_->getSession(req);
    if (!se || se->getValue("isLoggedIn") != "true")
    {
        json failureResp;
        failureResp["status"] = "fail";
        failureResp["message"] = "当前没有已登录的账号";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k403Forbidden, "Forbidden");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    // # 执行到这里，才终于 200 OK
    std::string uid_str = se->getValue("userId");
    json successResp;

    successResp["status"] = "success";
    successResp["mesage"] = "yes, you are logged in";
    successResp["userId"] = uid_str;
    std::string successBody = successResp.dump(4);

    resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
    resp->setCloseConnection(false);
    resp->setContentType("application/json");
    resp->setContentLength(successBody.size());
    resp->setBody(successBody);
    return;
}

// # 数据库操作函数
int MyWebAppServer::queryUserId(const std::string &username, const std::string &password)
{
    // 前端用户传来账号密码，查找数据库是否有该账号密码
    // 使用预处理语句, 防止sql注入
    // TODO: 如何防止 SQL 注入
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "SELECT uid FROM users WHERE username = \'" + username + "\' AND password = \'" + password + "\'";
    MySQLResult *res = conn_sp->query(query_str);
    if (!res)
    {
        LOG_ERROR("MySQL query error");
        return -1;
    }
    else
    {
        if (res->num_rows() == 0)
            return -1;
        else
            return std::stoi(res->at_key(0, "uid"));
    }
}

std::string MyWebAppServer::queryUserData(const std::string &username)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "SELECT userdata FROM users WHERE username = \'" + username + "\'";
    MySQLResult *res = conn_sp->query(query_str);
    if (!res)
    {
        LOG_ERROR("MySQL query error");
        return "";
    }
    else
    {
        if (res->num_rows() == 0)
            return "";
        else
            return res->at_key(0, "userdata");
    }
}

bool MyWebAppServer::postUserData(const std::string &username, const std::string &userdata)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "UPDATE users SET userdata = \'" + userdata + "\' WHERE username = \'" + username + "\'";
    auto res = conn_sp->update(query_str);
    return res;
}

int MyWebAppServer::checkUserExists(const std::string &username)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "SELECT uid FROM users WHERE username = \'" + username + "\'";
    MySQLResult *res = conn_sp->query(query_str);
    if (!res)
    {
        LOG_ERROR("MySQL query error");
        return -1;
    }
    else
    {
        if (res->num_rows() == 0)
            return -1;
        else
            return std::stoi(res->at_key(0, "uid"));
    }
}

int MyWebAppServer::registerUser(const std::string &username, const std::string &password)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "INSERT INTO users (username, password) VALUES (\'" + username + "\', \'" + password + "\')";
    auto res = conn_sp->update(query_str);
    if (!res)
    {
        LOG_ERROR("MySQL update error");
        throw std::runtime_error("Failed to register user");
    }
    else
    {
        LOG_INFO("User %s registered successfully", username.c_str());
    }

    int uid = checkUserExists(username);
    return uid;
}

bool MyWebAppServer::postArticle(unsigned int uid, const std::string &title, const std::string &content)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();

    std::string escaped_title;
    std::string escaped_content;

    // Properly escape the strings
    if (conn_sp->convertToEscapedString(title, escaped_title) == false ||
        conn_sp->convertToEscapedString(content, escaped_content) == false)
    {
        return false;
    }

    unsigned long long current_time = time(nullptr);

    std::string query_str = "INSERT INTO articles (uid, time, title, content) VALUES (" +
                            std::to_string(uid) + ", " +
                            std::to_string(current_time) + ", '" +
                            escaped_title + "', '" +
                            escaped_content + "')";

    auto res = conn_sp->update(query_str);

    return res;
}

bool MyWebAppServer::modifyArticle(unsigned long long aid, const std::string &content)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();

    std::string escaped_content;

    // Properly escape the strings
    if (conn_sp->convertToEscapedString(content, escaped_content) < 0)
    {
        return false;
    }

    std::string query_str = "UPDATE articles SET content = '" +
                            escaped_content + "' WHERE aid = " + std::to_string(aid);

    auto res = conn_sp->update(query_str);

    return res;
}

bool MyWebAppServer::deleteArticle(unsigned long long aid)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();

    std::string query_str = "DELETE FROM articles WHERE aid = " + std::to_string(aid);

    auto res = conn_sp->update(query_str);

    return res;
}

bool MyWebAppServer::checkUidofAid(unsigned long long aid, unsigned int &uid)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "SELECT uid FROM articles WHERE aid = " + std::to_string(aid);
    MySQLResult *res = conn_sp->query(query_str);
    if (!res)
    {
        LOG_ERROR("MySQL query error");
        return false;
    }
    else
    {
        if (res->num_rows() == 0)
            return false;
        else
        {
            uid = std::stoi(res->at(0, 0));
            return true;
        }
    }
}

std::shared_ptr<json> MyWebAppServer::articlesOfUser(unsigned int uid)
{

    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "SELECT aid,time,title,content FROM articles WHERE uid = " + std::to_string(uid);
    MySQLResult *res = conn_sp->query(query_str);
    if (!res)
    {
        LOG_ERROR("MySQL query error");
        return nullptr;
    }
    else
    {
        if (res->num_rows() == 0)
            return nullptr;
        else
        {
            std::shared_ptr<json> res_json = std::make_shared<json>();
            for (int i = 0; i < res->num_rows(); ++i)
            {
                (*res_json)[i]["aid"] = res->row(i)[0];
                (*res_json)[i]["time"] = res->row(i)[1];
                (*res_json)[i]["title"] = res->row(i)[2];
                (*res_json)[i]["content"] = res->row(i)[3];
            }
            return res_json;
        }
    }
}

std::shared_ptr<json> MyWebAppServer::latestArticles(int num)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "SELECT aid,time,title,content,articles.uid,username FROM articles LEFT JOIN users ON articles.uid = users.uid ORDER BY time DESC LIMIT " + std::to_string(num);
    MySQLResult *res = conn_sp->query(query_str);
    if (!res)
    {
        LOG_ERROR("MySQL query error");
        return nullptr;
    }
    else
    {
        if (res->num_rows() == 0)
            return nullptr;
        else
        {
            std::shared_ptr<json> res_json = std::make_shared<json>();
            for (int i = 0; i < res->num_rows(); ++i)
            {
                (*res_json)[i]["aid"] = res->row(i)[0];
                (*res_json)[i]["time"] = res->row(i)[1];
                (*res_json)[i]["title"] = res->row(i)[2];
                (*res_json)[i]["content"] = res->row(i)[3];
                (*res_json)[i]["uid"] = res->row(i)[4];
                (*res_json)[i]["username"] = res->row(i)[5];
            }
            return res_json;
        }
    }
}

std::shared_ptr<json> MyWebAppServer::listUsers()
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "SELECT users.uid,username,COUNT(aid) num_of_articles FROM articles LEFT JOIN users ON articles.uid = users.uid GROUP BY uid";
    MySQLResult *res = conn_sp->query(query_str);
    if (!res)
    {
        LOG_ERROR("MySQL query error");
        return nullptr;
    }
    else
    {
        if (res->num_rows() == 0)
            return nullptr;
        else
        {
            std::shared_ptr<json> res_json = std::make_shared<json>();
            for (int i = 0; i < res->num_rows(); ++i)
            {
                (*res_json)[i]["uid"] = res->row(i)[0];
                (*res_json)[i]["username"] = res->row(i)[1];
                (*res_json)[i]["num_of_articles"] = res->row(i)[2];
            }
            return res_json;
        }
    }
}