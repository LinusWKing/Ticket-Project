cmake_minimum_required(VERSION 3.14)
project(ticket_system VERSION 1.1.1)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 20)

add_executable(ticket_project main.cpp)

add_library(libs STATIC IMPORTED)
set_target_properties(libs PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/build/lib/libgcc_s_seh-1.dll)
target_link_libraries(ticket_project libs)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
