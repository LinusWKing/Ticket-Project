cmake_minimum_required(VERSION 3.14)
project(ticket_system)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_library(sqlite sqlite/sqlite3.c sqlite/sqlite3.h)
add_executable(ticket_project main.cpp)
add_executable(proj_test test.cc)

target_link_libraries(proj_test gtest_main)
target_link_libraries(ticket_project sqlite)

include_directories(sqlite)


include(GoogleTest)
gtest_discover_tests(proj_test)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
