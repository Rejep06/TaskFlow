cmake_minimum_required(VERSION 3.16)
project(server)

include(FetchContent)

# ---- cpp-httplib ----
# Disable non-blocking getaddrinfo on toolchains that don't expose
# GetAddrInfoExCancel (e.g. some MinGW targets)
set(HTTPLIB_USE_NON_BLOCKING_GETADDRINFO OFF CACHE BOOL "Disable non-blocking getaddrinfo" FORCE)
set(HTTPLIB_USE_ZSTD OFF CACHE BOOL "Disable zstd" FORCE)

FetchContent_Declare(
    httplib
    URL https://github.com/yhirose/cpp-httplib/archive/refs/heads/master.zip
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
