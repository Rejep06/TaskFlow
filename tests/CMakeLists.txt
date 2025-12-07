cmake_minimum_required(VERSION 3.16)
project(tests)

include(FetchContent)

# ---- GoogleTest ----
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# ---- Тестовый exe ----
add_executable(run_tests test_tasks.cpp)

# ---- Линкуем core и gtest ----
target_link_libraries(run_tests PRIVATE core GTest::gtest GTest::gtest_main)

add_test(NAME unit_tests COMMAND $<TARGET_FILE:run_tests>)
