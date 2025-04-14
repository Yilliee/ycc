cmake_minimum_required(VERSION 3.14)
project(ycc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enforce strict compiler warnings
add_compile_options(-Wall -Wextra -Werror)

# Add the root directory as an include directory
include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_FILES
    ycc.cpp

    filemanager/constants.h
    filemanager/reader.h
    filemanager/reader.cpp
    filemanager/writer.h
    filemanager/writer.cpp

    lexer/lexer.h
    lexer/lexer.cpp

    mapping/keyword.h
    mapping/operator.h

    modules/automaton.h
    modules/automaton.cpp
    modules/export_tokens.h
    modules/export_tokens.cpp
    modules/immutable_vector.h
    modules/immutable_vector.tcc
    modules/token.h
    modules/trie_automaton.h
    modules/trie_automaton.cpp

    preprocessor/preprocessor.h
    preprocessor/preprocessor.cpp

    tables/base_table.h
    tables/base_table.tcc
    tables/literal_table.h
    tables/symbol_table.h
)

# Set the working directory to the current directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Create the executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

