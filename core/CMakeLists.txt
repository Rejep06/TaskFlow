cmake_minimum_required(VERSION 3.15)
project(core VERSION 1.0.0)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка для Windows (UTF-8 поддержка)
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8 -fexec-charset=UTF-8")
endif()

# Все исходные файлы
set(SOURCES
    task.cpp
    task_manager.cpp
)

# Все заголовочные файлы
set(HEADERS
    task.hpp
    task_manager.hpp
    ITaskService.hpp
)


add_library(core STATIC ${SOURCES} ${HEADERS})

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Основное приложение
# add_executable(task_tracker
#     main.cpp
#     ${SOURCES}
#     ${HEADERS}
# )

# Устанавливаем инклюд директории (текущая папка)
# target_include_directories(task_tracker
#     PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
# )

# Настройка для разных типов сборки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(core PRIVATE DEBUG=1)
    message(STATUS "Building in DEBUG mode")
else()
    target_compile_definitions(core PRIVATE NDEBUG=1)
    message(STATUS "Building in RELEASE mode")
endif()

# ======================================
# КОМАНДЫ ДЛЯ ЗАПУСКА:
# ======================================
# 
# LINUX / macOS:
# ---------------
# 1. Создать папку build в текущей директории (если нет):
#    mkdir build
# 
# 2. Перейти в папку build:
#    cd build
# 
# 3. Запустить CMake конфигурацию:
#    cmake ..
# 
# 4. Собрать проект:
#    make
# 
# 5. Запустить приложение:
#    ./task_tracker
# 
# Быстрая сборка одной командой:
#    mkdir build && cd build && cmake .. && make && ./task_tracker
# 
# WINDOWS (PowerShell / Command Prompt):
# ---------------------------------------
# 1. Создать папку build (если нет):
#    mkdir build
# 
# 2. Перейти в папку build:
#    cd build
# 
# 3. Для MinGW (если установлен):
#    cmake .. -G "MinGW Makefiles"
#    cmake --build .
#    .\task_tracker.exe
# 
# 4. Для Visual Studio 2022:
#    cmake .. -G "Visual Studio 17 2022"
#    cmake --build . --config Release
#    .\Release\task_tracker.exe
# 
# 5. Для Visual Studio 2019:
#    cmake .. -G "Visual Studio 16 2019"
#    cmake --build . --config Release
#    .\Release\task_tracker.exe
# 
# УДАЛЕНИЕ ФАЙЛОВ СБОРКИ:
# -----------------------
# Linux/macOS:   rm -rf build
# Windows:       rmdir /s /q build
# 
# ============================================================================