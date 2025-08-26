#include "my_http/HttpServer.h"
#include "my_http/HttpRequest.h"
#include "my_http/HttpResponse.h"
#include "mymuduo/EventLoop.h"
#include "mymuduo/Logger.h"
#include "MyWebAppServer.h"

#include <iostream>
#include <map>
#include <functional>

int main(int argc, char *argv[])
{
  int numThreads = 3;
  if (argc > 1)
  {
    numThreads = atoi(argv[1]);
  }

  int blockqueue_size = 1024;
  Logger::instance()->init(LogLevel::TRACE, "./logs", ".log", blockqueue_size);


  EventLoop loop;
  MyWebAppServer server(&loop, InetAddress(50000, "0.0.0.0"), std::string("blog_server"));
  server.setThreadNum(numThreads);

  server.start();
  loop.loop();
}
