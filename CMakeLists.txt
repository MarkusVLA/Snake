cmake_minimum_required(VERSION 3.10)

project(snake VERSION 0.1)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)

# SFML submodule
add_subdirectory(libs/SDL)
include_directories(${SDL2_INCLUDE_DIR})



add_executable(snake
    # All source files
    src/main.c
)

target_link_libraries(snake SDL2)
