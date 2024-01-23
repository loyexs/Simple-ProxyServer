cmake_minimum_required(VERSION 3.11)

project(ProxyServer CXX)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Boost 1.82.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(ProxyServer 
  src/listener.hpp 
  src/session.hpp
  src/session.cpp
  src/main.cpp)

target_link_libraries(ProxyServer PRIVATE Threads::Threads) 