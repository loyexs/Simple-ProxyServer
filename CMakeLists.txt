cmake_minimum_required(VERSION 3.11)

project(ProxyServer CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.84.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(ProxyServer 
  src/tcp_manager.hpp
  src/tcp_manager.cpp
  src/session.hpp 
  src/session.cpp
  src/socket_io_handler.hpp
  src/socket_io_handler.cpp
  src/main.cpp)

target_link_libraries(ProxyServer PRIVATE Threads::Threads) 