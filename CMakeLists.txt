cmake_minimum_required(VERSION 3.14)
project(ticket_system VERSION 1.1.1)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 20)

add_executable(ticket_project main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
