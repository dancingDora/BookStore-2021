cmake_minimum_required(VERSION 3.16)
project(bookstore)

set(CMAKE_CXX_STANDARD 20)
#add_compile_options(-std=c++2a -fconcepts)

add_executable(code src/main.cpp src/blocklist.cpp src/usermanager.cpp src/bookstore.cpp)
