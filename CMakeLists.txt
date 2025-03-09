cmake_minimum_required(VERSION 3.14)
project(ycc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enforce strict compiler warnings
add_compile_options(-Wall -Wextra -Werror)

# Add the root directory as an include directory
include_directories(${CMAKE_SOURCE_DIR})

# List all source files manually
set(SOURCE_FILES
    ycc.cpp
    filemanager/reader.cpp
    filemanager/writer.cpp
    lexer/lexer.cpp
    preprocessor/preprocessor.cpp
)

# Create the executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

