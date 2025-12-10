cmake_minimum_required(VERSION 3.15)
project(TaskTracker VERSION 1.0.0)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка для Windows (UTF-8 поддержка)
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8 -fexec-charset=UTF-8")
endif()

# Все исходные файлы
set(SOURCES
    console_app.cpp
)

# Все заголовочные файлы
set(HEADERS
    console_app.hpp
)

# Основное приложение
add_executable(task_tracker
    main.cpp
    ${SOURCES}
    ${HEADERS}
)

# Устанавливаем инклюд директории (текущая папка)
target_include_directories(task_tracker
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)


target_link_libraries(task_tracker PRIVATE core)

target_include_directories(task_tracker PRIVATE ${CMAKE_SOURCE_DIR}/core)

# Настройка для разных типов сборки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(task_tracker PRIVATE DEBUG=1)
    message(STATUS "Building in DEBUG mode")
else()
    target_compile_definitions(task_tracker PRIVATE NDEBUG=1)
    message(STATUS "Building in RELEASE mode")
endif()