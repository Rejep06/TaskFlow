cmake_minimum_required(VERSION 3.16)
project(TaskFlowTests VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ---- GoogleTest ----
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# ---- Тестовый exe ----
add_executable(run_tests
    test_tasks.cpp
)

# ---- Include directories для тестов ----
target_include_directories(run_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/utils
)

# ---- Линкуем core и gtest ----
target_link_libraries(run_tests PRIVATE core GTest::gtest GTest::gtest_main)

# ---- Добавляем тест в CTest ----
enable_testing()
add_test(NAME unit_tests COMMAND $<TARGET_FILE:run_tests>)
