cmake_minimum_required(VERSION 3.16)
project(TaskManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# Директории проекта
set(INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/core"
    "${CMAKE_SOURCE_DIR}/server"
    "${CMAKE_SOURCE_DIR}/server/utils"
    "${CMAKE_SOURCE_DIR}/client"
    "${CMAKE_SOURCE_DIR}/tests"
)

# Формируем строку с include путями для cppcheck
set(CPPCHECK_INCLUDE_ARGS "")
foreach(dir IN LISTS INCLUDE_DIRS)
    list(APPEND CPPCHECK_INCLUDE_ARGS "-I${dir}")
endforeach()

# Настройка cppcheck
find_program(CPPCHECK_EXE NAMES "cppcheck")

if(CPPCHECK_EXE)
    message(STATUS "Found cppcheck: ${CPPCHECK_EXE}")

    # Собираем все исходники
    file(GLOB_RECURSE PROJECT_SOURCES
        "${CMAKE_SOURCE_DIR}/core/*.cpp"
        "${CMAKE_SOURCE_DIR}/server/*.cpp"
        "${CMAKE_SOURCE_DIR}/server/utils/*.cpp"
        "${CMAKE_SOURCE_DIR}/client/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/*.cpp"
    )

    add_custom_target(lint
        COMMAND "${CPPCHECK_EXE}" --enable=all --inconclusive --quiet --suppress=missingIncludeSystem
                ${CPPCHECK_INCLUDE_ARGS} ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cppcheck on project sources..."
    )
else()
    message(WARNING "cppcheck not found!")
endif()
