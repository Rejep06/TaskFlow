cmake_minimum_required(VERSION 3.16)
project(TaskManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(core)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)
