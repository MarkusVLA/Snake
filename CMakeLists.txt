
cmake_minimum_required(VERSION 3.10)

project(snake VERSION 0.1)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)

add_executable(snake
    # All source files
    src/main.c
)