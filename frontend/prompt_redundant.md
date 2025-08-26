# prompts to LLM:

I'm a C++ backend developer and I'm working on a web application. Now I have created several backend callbacks to handle user requests. Can you please provide me with the frontend code for the web application?

The following are the backend callbacks and their descriptions:

## how those callbacks are registered in my backend router, note how they are bound with a method-path pair

server_.Get(std::string("/"), std::bind(&MyWebAppServer::indexCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Post(std::string("/login"), std::bind(&MyWebAppServer::loginCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Post(std::string("/register"), std::bind(&MyWebAppServer::registerCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Get(std::string("/logout"), std::bind(&MyWebAppServer::logoutCallback, this, std::placeholders::_1, std::placeholders::_2));

// these endpoints are used by logged in users:
server_.Post(std::string("/post_article"), std::bind(&MyWebAppServer::post_articleCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Post(std::string("/modify_article"), std::bind(&MyWebAppServer::modify_articleCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Post(std::string("/delete_article"), std::bind(&MyWebAppServer::delete_articleCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Get(std::string("/list_my_articles"), std::bind(&MyWebAppServer::list_my_articleCallback, this, std::placeholders::_1, std::placeholders::_2));

// these endpoints are used by all users:
server_.Get(std::string("/list_latest_articles"), std::bind(&MyWebAppServer::list_latest_articlesCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Get(std::string("/list_users"), std::bind(&MyWebAppServer::list_user_articlesCallback, this, std::placeholders::_1, std::placeholders::_2));
server_.Post(std::string("/list_user_articles"), std::bind(&MyWebAppServer::list_user_articlesCallback, this, std::placeholders::_1, std::placeholders::_2));

## void indexCallback(const HttpRequest &req, HttpResponse *resp);
simply returns the index.html file to the client. 
this index.html file should be the frontend code for the web application, which is what you should write for me.

The request line would be:

GET / HTTP/1.1

### What logic should be implemented in the frontend code:

There are three regions displayed in the main page.

1. the user list region
  - list all the user names and their number of articles 
  - each user name is clickable, which will direct to the page displaying all the articles of that user when clicked

2. the latest articles region
  - list all the latest articles
  - display the full title, created time, author name of each article
  - as for the content, each one will only display a small part of the content as a preview
  - each article is clickable, which will direct to the page display the full content of that article when clicked

3. the login information region
  - if the user is not logged in, there are options to login or register a new account
  - if the user is logged in, there will diaplay:
    - the user name and number of articles 
    - an option to publish a new article
    - an option to logout
    - an entrence directs to the page displaying all the articles of that logged in user

The page to edit for publishing or modifying an article would contain:
  - input boxes to input title and contents
  - if it's for publishing an new article, there would be a "publish" button
  - if it's for modifying an old article, the title and content boxes would already be filled with the old text, and there would be a "publish modification" button.

The page displaying all the articles of a user will generally display:
  - a list of titles of those articles, with created time
  - each one is clickable, which will directs to the page displaying the full content of that article
In addition, if there is a logged in user and that page is the page listing all the articles of that logged in user, there will additionally display:
  - for each of those articles, there are options to delete that article, or modify that article

The page displaying the full content of that article will generally display:
  - the title, full content, created time, author of that article
In addition, if there is a logged in user and that article belongs that logged in user, there will additionally display:
  - options to delete, or modify that article


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
    "username": <string>,
    "password": <string>
}

If there is a format error in the request, the server will return 400 Bad Request response.
If the username or password is incorrect, the server will return 401 Unauthorized response.
If there is already a user logged in, the server will return 403 Forbidden response.
If the login is successful, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK
Set-Cookie: sessionId=<session_id>; Path=/; HttpOnly

{
    "status": "success",
    "message": <string>,
    "userId": <string>
}

## void registerCallback(const HttpRequest &req, HttpResponse *resp);
this POST request format must contain the following information inorder to get 200 OK response from the server:

POST /register HTTP/1.1
Content-Type: application/json

{
    "username": <string>,
    "password": <string>
}

If there is a format error in the request, the server will return 400 Bad Request response.
If the username already exists or the username or password is too long, the server will return 403 Forbidden response.
If the registration is successful, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "userId": <string>
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
    "message": <string>
}

## void postArticleCallback(const HttpRequest &req, HttpResponse *resp);
this POST request format must contain the following information inorder to get 200 OK response from the server:

POST /post_article HTTP/1.1
Content-Type: application/json
Cookie: sessionId=<session_id>

{
    "title": <string>,
    "content": <string>
}

If there is a format error in the request, the server will return 400 Bad Request response.
If the user is not logged in, the server will return 403 Forbidden response.
If the article is successfully added to the database, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": <string>
}


## void modify_articleCallback(const HttpRequest &req, HttpResponse *resp);
this POST request format must contain the following information inorder to get 200 OK response from the server:

GET /modify_article HTTP/1.1
Cookie: sessionId=<session_id>

{
    "aid": <string>,
    "content": <string>
}

If there is a format error in the request, the server will return 400 Bad Request response.
If the user is not logged in, the server will return 403 Forbidden response.
If the userdata is successfully retrieved from the database, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": <string>,
}

## void delete_articleCallback(const HttpRequest &req, HttpResponse *resp);
this POST request format must contain the following information inorder to get 200 OK response from the server:

GET /delete_article HTTP/1.1
Cookie: sessionId=<session_id>

{
    "aid": <string>,
}

If there is a format error in the request, the server will return 400 Bad Request response.
If the user is not logged in, the server will return 403 Forbidden response.
If the userdata is successfully retrieved from the database, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": <string>,
}


## void list_my_articles(const HttpRequest &req, HttpResponse *resp);
this GET request format must contain the following information inorder to get 200 OK response from the server:

GET /delete_article HTTP/1.1
Cookie: sessionId=<session_id>

If the user is not logged in, the server will return 403 Forbidden response.
If the userdata is successfully retrieved from the database, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": <string>,
    "articles": 
        [
            {
            "aid": <string>,
            "time": <string>,
            "title": <string>,
            "content": <string>
            }, 
            ...
        ]
}

## void list_latest_articles(const HttpRequest &req, HttpResponse *resp);
this GET request format must contain the following information inorder to get 200 OK response from the server:

GET /list_latest_articles HTTP/1.1

If the userdata is successfully retrieved from the database, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": <string>,
    "articles": 
        [
            {
            "aid": <string>,
            "time": <string>,
            "title": <string>,
            "content": <string>,
            "uid": <string>,
            "username": <string>
            },
            ...
        ]

}

## void list_usersCallback(const HttpRequest &req, HttpResponse *resp);
this GET request format must contain the following information inorder to get 200 OK response from the server:

GET /list_users HTTP/1.1

If the userdata is successfully retrieved from the database, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": <string>,
    "users": 
        [
            {
            "uid": <string>,
            "username": <string>,
            "num_of_articles": <string>
            },
            ...
        ]
}

## void list_user_articlesCallback(const HttpRequest &req, HttpResponse *resp);
this POST request format must contain the following information inorder to get 200 OK response from the server:

GET /list_user_articles HTTP/1.1

{
    "uid": <string>,
}

If there is a format error in the request, the server will return 400 Bad Request response.
If the userdata is successfully retrieved from the database, the server will return 200 OK response with the following JSON data:

HTTP/1.1 200 OK

{
    "status": "success",
    "message": <string>,
    "articles": 
        [
            {
            "aid": <string>,
            "time": <string>,
            "title": <string>,
            "content": <string>
            }, 
            ...
        ]
}

