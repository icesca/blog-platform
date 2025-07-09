# bug to fix

- 可能是因为手机浏览器退出窗口后不保存 cookie? 在手机端登录后，退出浏览器，再次进入，会出现登录状态丢失的情况。
  这样就会出现该登录状态在服务端有记录，但客户端因为丢失了 cookie 而无法获取到登录状态。
 
  解决方案 todo:
  在会话管理的逻辑处处理：如果再次登录的账户密码正确，即使 login request 中没有 cookie, 也返回已有的 session 的 cookie 作为登录响应。

# feature to add

- 增加定时器关闭超时的 session，减少服务器资源占用 （storage_.load() 并不能完全处理，因为它只有在通过确切的 sessionId 访问时才会清除过期 session）