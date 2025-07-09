#include "MyWebAppServer.h"
#include <my_http/conn_pool/ConnectionPool.h>
#include <my_http/JsonUtil.h>
#include <memory>

const static char favicon[555] = {
    '\x89',
    'P',
    'N',
    'G',
    '\xD',
    '\xA',
    '\x1A',
    '\xA',
    '\x0',
    '\x0',
    '\x0',
    '\xD',
    'I',
    'H',
    'D',
    'R',
    '\x0',
    '\x0',
    '\x0',
    '\x10',
    '\x0',
    '\x0',
    '\x0',
    '\x10',
    '\x8',
    '\x6',
    '\x0',
    '\x0',
    '\x0',
    '\x1F',
    '\xF3',
    '\xFF',
    'a',
    '\x0',
    '\x0',
    '\x0',
    '\x19',
    't',
    'E',
    'X',
    't',
    'S',
    'o',
    'f',
    't',
    'w',
    'a',
    'r',
    'e',
    '\x0',
    'A',
    'd',
    'o',
    'b',
    'e',
    '\x20',
    'I',
    'm',
    'a',
    'g',
    'e',
    'R',
    'e',
    'a',
    'd',
    'y',
    'q',
    '\xC9',
    'e',
    '\x3C',
    '\x0',
    '\x0',
    '\x1',
    '\xCD',
    'I',
    'D',
    'A',
    'T',
    'x',
    '\xDA',
    '\x94',
    '\x93',
    '9',
    'H',
    '\x3',
    'A',
    '\x14',
    '\x86',
    '\xFF',
    '\x5D',
    'b',
    '\xA7',
    '\x4',
    'R',
    '\xC4',
    'm',
    '\x22',
    '\x1E',
    '\xA0',
    'F',
    '\x24',
    '\x8',
    '\x16',
    '\x16',
    'v',
    '\xA',
    '6',
    '\xBA',
    'J',
    '\x9A',
    '\x80',
    '\x8',
    'A',
    '\xB4',
    'q',
    '\x85',
    'X',
    '\x89',
    'G',
    '\xB0',
    'I',
    '\xA9',
    'Q',
    '\x24',
    '\xCD',
    '\xA6',
    '\x8',
    '\xA4',
    'H',
    'c',
    '\x91',
    'B',
    '\xB',
    '\xAF',
    'V',
    '\xC1',
    'F',
    '\xB4',
    '\x15',
    '\xCF',
    '\x22',
    'X',
    '\x98',
    '\xB',
    'T',
    'H',
    '\x8A',
    'd',
    '\x93',
    '\x8D',
    '\xFB',
    'F',
    'g',
    '\xC9',
    '\x1A',
    '\x14',
    '\x7D',
    '\xF0',
    'f',
    'v',
    'f',
    '\xDF',
    '\x7C',
    '\xEF',
    '\xE7',
    'g',
    'F',
    '\xA8',
    '\xD5',
    'j',
    'H',
    '\x24',
    '\x12',
    '\x2A',
    '\x0',
    '\x5',
    '\xBF',
    'G',
    '\xD4',
    '\xEF',
    '\xF7',
    '\x2F',
    '6',
    '\xEC',
    '\x12',
    '\x20',
    '\x1E',
    '\x8F',
    '\xD7',
    '\xAA',
    '\xD5',
    '\xEA',
    '\xAF',
    'I',
    '5',
    'F',
    '\xAA',
    'T',
    '\x5F',
    '\x9F',
    '\x22',
    'A',
    '\x2A',
    '\x95',
    '\xA',
    '\x83',
    '\xE5',
    'r',
    '9',
    'd',
    '\xB3',
    'Y',
    '\x96',
    '\x99',
    'L',
    '\x6',
    '\xE9',
    't',
    '\x9A',
    '\x25',
    '\x85',
    '\x2C',
    '\xCB',
    'T',
    '\xA7',
    '\xC4',
    'b',
    '1',
    '\xB5',
    '\x5E',
    '\x0',
    '\x3',
    'h',
    '\x9A',
    '\xC6',
    '\x16',
    '\x82',
    '\x20',
    'X',
    'R',
    '\x14',
    'E',
    '6',
    'S',
    '\x94',
    '\xCB',
    'e',
    'x',
    '\xBD',
    '\x5E',
    '\xAA',
    'U',
    'T',
    '\x23',
    'L',
    '\xC0',
    '\xE0',
    '\xE2',
    '\xC1',
    '\x8F',
    '\x0',
    '\x9E',
    '\xBC',
    '\x9',
    'A',
    '\x7C',
    '\x3E',
    '\x1F',
    '\x83',
    'D',
    '\x22',
    '\x11',
    '\xD5',
    'T',
    '\x40',
    '\x3F',
    '8',
    '\x80',
    'w',
    '\xE5',
    '3',
    '\x7',
    '\xB8',
    '\x5C',
    '\x2E',
    'H',
    '\x92',
    '\x4',
    '\x87',
    '\xC3',
    '\x81',
    '\x40',
    '\x20',
    '\x40',
    'g',
    '\x98',
    '\xE9',
    '6',
    '\x1A',
    '\xA6',
    'g',
    '\x15',
    '\x4',
    '\xE3',
    '\xD7',
    '\xC8',
    '\xBD',
    '\x15',
    '\xE1',
    'i',
    '\xB7',
    'C',
    '\xAB',
    '\xEA',
    'x',
    '\x2F',
    'j',
    'X',
    '\x92',
    '\xBB',
    '\x18',
    '\x20',
    '\x9F',
    '\xCF',
    '3',
    '\xC3',
    '\xB8',
    '\xE9',
    'N',
    '\xA7',
    '\xD3',
    'l',
    'J',
    '\x0',
    'i',
    '6',
    '\x7C',
    '\x8E',
    '\xE1',
    '\xFE',
    'V',
    '\x84',
    '\xE7',
    '\x3C',
    '\x9F',
    'r',
    '\x2B',
    '\x3A',
    'B',
    '\x7B',
    '7',
    'f',
    'w',
    '\xAE',
    '\x8E',
    '\xE',
    '\xF3',
    '\xBD',
    'R',
    '\xA9',
    'd',
    '\x2',
    'B',
    '\xAF',
    '\x85',
    '2',
    'f',
    'F',
    '\xBA',
    '\xC',
    '\xD9',
    '\x9F',
    '\x1D',
    '\x9A',
    'l',
    '\x22',
    '\xE6',
    '\xC7',
    '\x3A',
    '\x2C',
    '\x80',
    '\xEF',
    '\xC1',
    '\x15',
    '\x90',
    '\x7',
    '\x93',
    '\xA2',
    '\x28',
    '\xA0',
    'S',
    'j',
    '\xB1',
    '\xB8',
    '\xDF',
    '\x29',
    '5',
    'C',
    '\xE',
    '\x3F',
    'X',
    '\xFC',
    '\x98',
    '\xDA',
    'y',
    'j',
    'P',
    '\x40',
    '\x0',
    '\x87',
    '\xAE',
    '\x1B',
    '\x17',
    'B',
    '\xB4',
    '\x3A',
    '\x3F',
    '\xBE',
    'y',
    '\xC7',
    '\xA',
    '\x26',
    '\xB6',
    '\xEE',
    '\xD9',
    '\x9A',
    '\x60',
    '\x14',
    '\x93',
    '\xDB',
    '\x8F',
    '\xD',
    '\xA',
    '\x2E',
    '\xE9',
    '\x23',
    '\x95',
    '\x29',
    'X',
    '\x0',
    '\x27',
    '\xEB',
    'n',
    'V',
    'p',
    '\xBC',
    '\xD6',
    '\xCB',
    '\xD6',
    'G',
    '\xAB',
    '\x3D',
    'l',
    '\x7D',
    '\xB8',
    '\xD2',
    '\xDD',
    '\xA0',
    '\x60',
    '\x83',
    '\xBA',
    '\xEF',
    '\x5F',
    '\xA4',
    '\xEA',
    '\xCC',
    '\x2',
    'N',
    '\xAE',
    '\x5E',
    'p',
    '\x1A',
    '\xEC',
    '\xB3',
    '\x40',
    '9',
    '\xAC',
    '\xFE',
    '\xF2',
    '\x91',
    '\x89',
    'g',
    '\x91',
    '\x85',
    '\x21',
    '\xA8',
    '\x87',
    '\xB7',
    'X',
    '\x7E',
    '\x7E',
    '\x85',
    '\xBB',
    '\xCD',
    'N',
    'N',
    'b',
    't',
    '\x40',
    '\xFA',
    '\x93',
    '\x89',
    '\xEC',
    '\x1E',
    '\xEC',
    '\x86',
    '\x2',
    'H',
    '\x26',
    '\x93',
    '\xD0',
    'u',
    '\x1D',
    '\x7F',
    '\x9',
    '2',
    '\x95',
    '\xBF',
    '\x1F',
    '\xDB',
    '\xD7',
    'c',
    '\x8A',
    '\x1A',
    '\xF7',
    '\x5C',
    '\xC1',
    '\xFF',
    '\x22',
    'J',
    '\xC3',
    '\x87',
    '\x0',
    '\x3',
    '\x0',
    'K',
    '\xBB',
    '\xF8',
    '\xD6',
    '\x2A',
    'v',
    '\x98',
    'I',
    '\x0',
    '\x0',
    '\x0',
    '\x0',
    'I',
    'E',
    'N',
    'D',
    '\xAE',
    'B',
    '\x60',
    '\x82',
};

void MyWebAppServer::initRouter()
{
    // Q: why this doesn't work
    // server_.Get(std::string("/"), std::bind(MyWebAppServer::entryCallback, this, std::placeholders::_1, std::placeholders::_2));

    server_.Get(std::string("/test"), std::bind(&MyWebAppServer::testCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Get(std::string("/favicon.ico"), std::bind(&MyWebAppServer::faviconCallback, this, std::placeholders::_1, std::placeholders::_2));

    server_.Get(std::string("/"), std::bind(&MyWebAppServer::indexCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Post(std::string("/login"), std::bind(&MyWebAppServer::loginCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Get(std::string("/logout"), std::bind(&MyWebAppServer::logoutCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Get(std::string("/userdata"), std::bind(&MyWebAppServer::userdataCallback, this, std::placeholders::_1, std::placeholders::_2));
    server_.Post(std::string("/postdata"), std::bind(&MyWebAppServer::postdataCallback, this, std::placeholders::_1, std::placeholders::_2));
}

void MyWebAppServer::initSession()
{

    session_manager_ = std::make_unique<SessionManager>(std::make_unique<MemorySessionStorage>());
}

// # 不需要会话管理的路径：
// /test
// /favicon.ico
void MyWebAppServer::testCallback(const HttpRequest &req, HttpResponse *resp)
{
    resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
    resp->setContentType("text/html");
    resp->addHeader("Server", "Muduo");
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

void MyWebAppServer::faviconCallback(const HttpRequest &req, HttpResponse *resp)
{
    resp->setStatusLine(req.getVersionStr(), HttpResponse::k200Ok, "OK");
    resp->setContentType("image/png");
    resp->setBody(std::string(favicon, sizeof favicon));
    resp->setContentLength(sizeof favicon);
}

// # 需要会话管理的路径：
void MyWebAppServer::indexCallback(const HttpRequest &req, HttpResponse *resp)
{
    std::string reqFile;
    reqFile.append("../frontend/index.html");
    FileUtil fileOperater(reqFile);
    if (!fileOperater.isValid())
    {
        LOG_ERROR("%s does not exist", reqFile.c_str());
        fileOperater.resetDefaultFile(); // 404 NOT FOUND
    }

    // Q: why std::vector<chat> then std::string
    std::vector<char> buffer(fileOperater.size());
    fileOperater.readFile(buffer); // 读出文件数据
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

void MyWebAppServer::userdataCallback(const HttpRequest &req, HttpResponse *resp)
{
    /*
    # 可能的回应：
    400 Bad Request (TODO: 用 json 获取 user 多条信息中的某一条)
      格式错误
      处理 login, 那么就必然要先从 request body 中获取账号密码
    403 Forbidden
      rep 中不带有有效的 sessionId
    200 OK
      成功获取到 userdata, 通过 json 返回
    */

    // # 检查 403 Forbidden 错误
    std::shared_ptr<Session> se = session_manager_->sessionExist(req);
    if (!se || se->getValue("isLoggedIn") != "true")
    {
        json failureResp;
        failureResp["status"] = "error";
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

        // 真正执行的任务, 其余和 logoutCallback() 的逻辑类似
        std::string username = se->getValue("username");
        std::string userdata = queryUserData(username);
        ///

        json response;
        response["status"] = "success";
        response["message"] = "got userdata of " + username;
        response["userdata"] = userdata;
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
        failureResp["status"] = "error";
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

void MyWebAppServer::postdataCallback(const HttpRequest &req, HttpResponse *resp)
{
    /*
    # 可能的回应：
    400 Bad Request
      格式错误
    403 Forbidden
      rep 中不带有有效的 sessionId
    200 OK
      成功获取到 userdata, 通过 json 返回
    */

    // # 检查 400 Bad Request 错误
    std::string content_type = req.getHeader("Content-Type");
    std::string body = req.getBody();

    if (content_type.empty() || content_type != "application/json" || body.empty())
    {
        LOG_INFO("get content:\n%s", body.c_str());

        json failureResp;
        failureResp["status"] = "error";
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
    std::shared_ptr<Session> se = session_manager_->sessionExist(req);
    if (!se || se->getValue("isLoggedIn") != "true")
    {
        json failureResp;
        failureResp["status"] = "error";
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
        std::string username = se->getValue("username");
        std::string userdata = parsed["userdata"];
        // 真正重要的业务逻辑就这些
        auto res = postUserData(username, userdata);
        ///

        json successResp;
        if (res)
        {
            successResp["status"] = "success";
            successResp["mesage"] = "posted userdata";
        }
        else
        {
            successResp["status"] = "error";
            successResp["mesage"] = "postUserData failed";
        }
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
        json failureResp;
        failureResp["status"] = "error";
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
      该请求中带有有效的sessionId(能够在session_storage中找到并且是loggin状态)
      也就是说客户端已经有了一个处于连接状态的会话
    401 Unauthorized
      在查找数据库后，发现账号密码不合法
    200 OK
      成功登录并创建会话，客户端会收到有效的 sessionId
    */

    // 1. 检查 400 Bad Request 错误
    std::string content_type = req.getHeader("Content-Type");
    std::string body = req.getBody();

    // body.empty() 的问题在 HttpContext::parseRequest() 中已经反馈了
    if (content_type.empty() || content_type != "application/json" || body.empty())
    {
        LOG_INFO("get content:\n%s", body.c_str());

        json failureResp;
        failureResp["status"] = "error";
        failureResp["message"] = "格式错误";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersionStr(), HttpResponse::k400BadRequest, "(TEST) Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }

    // # 2. 检查 403 Forbidden 错误
    std::shared_ptr<Session> se = session_manager_->sessionExist(req);
    if (se)
    {
        if (se->getValue("isLoggedIn") == "true")
        {
            json failureResp;
            failureResp["status"] = "error";
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
        json parsed = json::parse(req.getBody());
        std::string username = parsed["username"];
        std::string password = parsed["password"];

        int userId = queryUserId(username, password);

        // # 再次检查 403 Forbidden 错误,

        // 如果用户请求没有携带有效的 sessionId，但是携带了有效的 username 和 password，
        // 那么就检查可能会对相同的用户重复生成 sessionId,
        // 所以要维护一个已经登陆的 users 的 id 的列表
        if (logged_in_users_.count(userId) != 0 && logged_in_users_[userId] == true)
        {
            json failureResp;
            failureResp["status"] = "error";
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

        // # 3. 检查 401 Unauthorized 错误
        // 格式没有问题了，那么就 从 json 中提取用户名密码，
        // 在数据库中验证用户名密码的正确性
        if (userId == -1)
        {
            // 封装json数据
            json failureResp;
            failureResp["status"] = "error";
            failureResp["message"] = "Invalid username or password";
            std::string failureBody = failureResp.dump(4);

            resp->setStatusLine(req.getVersionStr(), HttpResponse::k401Unauthorized, "Unauthorized");
            resp->setCloseConnection(false);
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
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        // 可能是 json::parse() 失败
        json failureResp;
        failureResp["status"] = "error";
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
    std::shared_ptr<Session> se = session_manager_->sessionExist(req);
    if (!se || se->getValue("isLoggedIn") != "true")
    {
        json failureResp;
        failureResp["status"] = "error";
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
        session_manager_->destroySession(se->getId());
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
        failureResp["status"] = "error";
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

int MyWebAppServer::queryUserId(const std::string &username, const std::string &password)
{
    // 前端用户传来账号密码，查找数据库是否有该账号密码
    // 使用预处理语句, 防止sql注入
    // TODO: 如何防止 SQL 注入
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "SELECT id FROM users WHERE username = \"" + username + "\" AND password = \"" + password + "\"";
    MySQLResult *res = conn_sp->query(query_str);
#define DEBUG
#ifdef DEBUG
    std::cout << username << " " << password << '\n';
    std::cout << res << '\n';
#endif
    if (!res)
    {
        LOG_ERROR("MySQL query error\n");
        return -1;
    }
    else
    {
        if (res->getRowCount() == 0)
            return -1;
        else
            return std::stoi(res->getValue(0, "id"));
    }
}

std::string MyWebAppServer::queryUserData(const std::string &username)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "SELECT userdata FROM users WHERE username = \"" + username + "\"";
    MySQLResult *res = conn_sp->query(query_str);
    if (!res)
    {
        LOG_ERROR("MySQL query error\n");
        return "";
    }
    else
    {
        if (res->getRowCount() == 0)
            return "";
        else
            return res->getValue(0, "userdata");
    }
}

bool MyWebAppServer::postUserData(const std::string &username, const std::string &userdata)
{
    std::shared_ptr<Connection> conn_sp = ConnectionPool::getConnectionPool()->getConnection();
    std::string query_str = "UPDATE users SET userdata = \"" + userdata + "\" WHERE username = \"" + username + "\"";
    auto res = conn_sp->update(query_str);
    return res;
}
