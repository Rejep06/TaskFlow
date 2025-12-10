cmake_minimum_required(VERSION 3.15)
project(TaskTracker VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8 -fexec-charset=UTF-8")
endif()

include(FetchContent)

# ---- cpp-httplib ----
set(HTTPLIB_USE_ZSTD OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_OPENSSL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    httplib
    URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.15.3.zip
)
FetchContent_MakeAvailable(httplib)

# ---- nlohmann/json ----
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
)
FetchContent_MakeAvailable(json)


set(SOURCES
    console_app.cpp
)

set(HEADERS
    console_app.hpp
)

add_executable(task_tracker
    main.cpp
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(task_tracker PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/core
)

target_link_libraries(task_tracker PRIVATE
    core
    httplib
    nlohmann_json::nlohmann_json
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(task_tracker PRIVATE DEBUG=1)
    message(STATUS "Building in DEBUG mode")
else()
    target_compile_definitions(task_tracker PRIVATE NDEBUG=1)
    message(STATUS "Building in RELEASE mode")
endif()