cmake_minimum_required(VERSION 3.10)
project(AMG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# include dirs for your library and utf8cpp
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/external/utf8cpp/source
)

# find utf8proc
find_library(UTF8PROC_LIBRARY NAMES utf8proc REQUIRED)

# -----------------------------------------------------------------------------
# 1) build your main amg executable
# -----------------------------------------------------------------------------
set(SOURCES
  src/main.cpp
  src/tokenizer/tokenizer.cpp
)

add_executable(amg ${SOURCES})
target_link_libraries(amg PRIVATE ${UTF8PROC_LIBRARY})

# -----------------------------------------------------------------------------
# 2) enable testing
# -----------------------------------------------------------------------------
enable_testing()

# find GoogleTest (must be installed or provided via FetchContent)
find_package(GTest REQUIRED)

# -----------------------------------------------------------------------------
# 3) build the tokenizer_test executable
# -----------------------------------------------------------------------------
add_executable(tokenizer_test
  src/tokenizer/tokenizer.cpp           # or link against an amg library if you refactor
  tests/test_tokenizer.cpp
)

# link to GTest (gtest_main provides its own main()), plus utf8proc
target_link_libraries(tokenizer_test
  PRIVATE
    GTest::gtest_main
    ${UTF8PROC_LIBRARY}
)

# register the test with CTest
add_test(
  NAME Tokenizer
  COMMAND tokenizer_test
)
