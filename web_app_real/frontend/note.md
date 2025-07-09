# prompts to Claude:

I'm a backend developer and I'm working on a web application. Now I have created several backend callbacks to handle user requests. Can you please provide me with the frontend code for the web application?

The following are the backend callbacks and their descriptions:

## how those callbacks are registered in my backend router, note how they are bound various methods and paths
server_.Get(std::string("/"), std::bind(&MyWebAppServer::indexCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Post(std::string("/login"), std::bind(&MyWebAppServer::loginCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Post(std::string("/register"), std::bind(&MyWebAppServer::registerCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Get(std::string("/logout"), std::bind(&MyWebAppServer::logoutCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Get(std::string("/userdata"), std::bind(&MyWebAppServer::userdataCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Post(std::string("/postdata"), std::bind(&MyWebAppServer::postdataCallback, this, std::placeholders::_1, std::placeholders::_2));

## void indexCallback(const HttpRequest &req, HttpResponse *resp);
simply returns the index.html file to the client. 
this index.html file should be the frontend code for the web application.

What logic should be implemented in the frontend code:
- This is a single page web application 
- If a user is already logged in, the index.html file should display:
  - the user's username
  - a link to the userdata page
  - a link to the postdata page
  - a link to the logout page
- If a user is not logged in, the index.html file should display:
  - a link to the login page
  - a link to the register page
- Note:
 - close the tab window of the browser doesn't cause the user to be logged out

What style should the frontend code have:
- simple and clean design since I'm not a frontend developer
- responsive design for different screen sizes


## void loginCallback(const HttpRequest &req, HttpResponse *resp);
this POST request format must contain the following information inorder to get 200 OK response from the server:

POST /login HTTP/1.1
Content-Type: application/json

{
    "username": "your_username",
    "password": "your_password"
}

If there is a format error in the request, the server will return 400 Bad Request response.
If the username or password is incorrect, the server will return 401 Unauthorized response.
If there is already a user logged in, the server will return 403 Forbidden response.
If the login is successful, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK
Set-Cookie: sessionId=<session_id>; Path=/; HttpOnly

{
    "status": "success",
    "userId": "<some_id>"
}

## void registerCallback(const HttpRequest &req, HttpResponse *resp);
this POST request format must contain the following information inorder to get 200 OK response from the server:

POST /register HTTP/1.1
Content-Type: application/json

{
    "username": "your_username",
    "password": "your_password"
}

If there is a format error in the request, the server will return 400 Bad Request response.
If the username already exists or the username or password is too long, the server will return 403 Forbidden response.
If the registration is successful, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "userId": "<some_id>"
}

## void postdataCallback(const HttpRequest &req, HttpResponse *resp);
this POST request format must contain the following information inorder to get 200 OK response from the server:

POST /postdata HTTP/1.1
Content-Type: application/json
Cookie: sessionId=<session_id>

{
    "userdata": "your_userdata"
}

If there is a format error in the request, the server will return 400 Bad Request response.
If the user is not logged in, the server will return 403 Forbidden response.
If the postdata is successfully added to the database, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": "posted userdata"
}


## void userdataCallback(const HttpRequest &req, HttpResponse *resp);
this GET request format must contain the following information inorder to get 200 OK response from the server:

GET /userdata HTTP/1.1
Cookie: sessionId=<session_id>

If the user is not logged in, the server will return 403 Forbidden response.
If the userdata is successfully retrieved from the database, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": "got userdata of <username>",
    "userdata": "your_userdata"
}

## void logoutCallback(const HttpRequest &req, HttpResponse *resp);
this GET request format must contain the following information inorder to get 200 OK response from the server:

GET /logout HTTP/1.1
Cookie: sessionId=<session_id>

If the user is not logged in, the server will return 403 Forbidden response.
If the user is successfully logged out, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": "logged success"
}