cmake_minimum_required(VERSION 3.16)
project(server)

cmake_policy(SET CMP0135 NEW)
include(FetchContent)

# ---- cpp-httplib (stable) ----
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
    URL https://github.com/nlohmann/json/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(json)


add_executable(server
    main.cpp
    TaskController.cpp
    TaskRepository.cpp
    ./utils/ParserTime.cpp
)

target_include_directories(server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)


target_link_libraries(server PRIVATE
    core
    httplib
    nlohmann_json
)
