cmake_minimum_required(VERSION 2.8)
project(socket_test)
add_definitions(-std=c++11)

#set(CMAKE_VERBOSE_MAKEFILE ON)
#link_directories(/lib/x86_64-linux-gnu)
#find_package(Threads REQUIRED)

add_executable(send send.cpp constants.h)

add_executable(receive receive.cpp constants.h)
target_link_libraries (receive pthread)
