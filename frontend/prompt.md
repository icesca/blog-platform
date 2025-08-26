# prompts to LLM:

I'm a C++ backend developer and I'm working on a web application. It's basically a blog platform that allows users hold their own accounts, post and view articles. Now I have created several backend apis to handle user requests. Using those backend apis I provided, can you please provide me with the frontend code for the web application?

# what framework to use:

Please write this SPA code using Vue3, and make sure loading the vue library from CDN so I don't need to build it.

# What logic should be implemented in the frontend code:

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

What style should the frontend code have:
- simple and clean design since I'm not a frontend developer
- responsive design for different screen sizes

# list of backend apis

GET /
POST /login
POST /register
GET /logout

these endpoints are used by logged in users:

POST /post_article
POST /modify_article
POST /delete_article
GET /list_my_articles

these endpoints are used by all users:

GET /list_latest_articles
GET /list_users
POST /list_user_articles

GET /check_loggedin

# descriptions of apis
note: for those apis whose request body or response body I don't mention, there are simply no request body for GET request, 
and the response body for 200 OK response is simply:
{
    "status": "success",
    "message": <message_string>
}
the response body for other response codes is:
{
    "status": "fail",
    "message": <message_string>
}

## GET /

OK response:
simply returns the index.html file to the client. 
this index.html file should be the frontend code for the web application, which is what you should write for me.

## POST /login

request body:
{
    "username": <string>,
    "password": <string>
}

OK response body:
{
    "status": "success",
    "message": <string>,
    "userId": <string>
}

## POST /register

request body:
{
    "username": <string>,
    "password": <string>
}

OK response body:
{
    "status": "success",
    "message": <string>,
    "userId": <string>
}

## GET /logout

## POST /post_article

request body:
{
    "title": <string>,
    "content": <string>
}

## POST /modify_article

request body:
{
    "aid": <string>,
    "content": <string>
}

## POST /delete_article

request body:
{
    "aid": <string>,
}

## GET /list_my_articles

OK response body:
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

## GET /list_latest_articles

OK response body:
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

## GET /list_users

OK response body:
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

## POST /list_user_articles

request body:
{
    "uid": <string>,
}

OK response body:
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

## GET /check_loggedin

OK response body:
{
    "status": "success",
    "message": "yes, you are logged in",
    "userId": <string>
}

-----------------------------
# note

Cookie: sessionId=<session_id>
Set-Cookie: sessionId=<session_id>; Path=/; HttpOnly
会话处理相关的 Cookie 的信息不需要在提示词中提及，浏览器自动会处理，并不是由前端代码处理。

# 结果

相同的提示词分别给到了 ChatGPT, DeepSeek, Claude, 最终选用了结果最佳的 Claude 的版本。
