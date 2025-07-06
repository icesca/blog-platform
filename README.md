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
