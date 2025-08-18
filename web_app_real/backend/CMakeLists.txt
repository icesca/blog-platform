cmake_minimum_required(VERSION 3.5)

project(webapp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(webapp main.cc MyWebAppServer.cc)
add_executable(webapp_static main.cc MyWebAppServer.cc)

target_link_libraries(webapp my_http mymuduo)
target_link_libraries(webapp_static my_http.a mymuduo.a mysqlclient.a ssl.a crypto.a resolv.a z.a zstd.a)

# 怎么确定 webapp_static 的所有依赖的：
# 关键在 mysqlclient.
# ldd libmysqlclient.so 查看其依赖的动态链接库，改为静态库全部链接即可
