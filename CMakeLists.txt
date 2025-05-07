cmake_minimum_required(VERSION 3.10)
project(AMG)

set(CMAKE_CXX_STANDARD 17)

# Add your project's include directories
include_directories(include)
include_directories(external/utf8cpp/source)

# List of source files
set(SOURCES
    src/tokenizer.cpp
    src/main.cpp
)

# Define the executable
add_executable(amg ${SOURCES})

# Link the system-installed utf8proc
find_library(UTF8PROC_LIBRARY NAMES utf8proc REQUIRED)

# Link to utf8proc
target_link_libraries(amg PRIVATE ${UTF8PROC_LIBRARY})
