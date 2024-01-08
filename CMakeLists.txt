cmake_minimum_required(VERSION 3.27)

project(tic_tac_toe
    VERSION 0.1
    DESCRIPTION "A modern C library to prvide useful and low overhead abstractions"
    LANGUAGES C
)
# CMAKE Standard
set(CMAKE_C_STANDARD 11)
# Adding global flags for debug
set(CMAKE_C_FLAGS_DEBUG_INIT "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wreturn-type -Wswitched -Wunused -Werror -fsanitize=address -g")
# Adding global flags for release
set(CMAKE_C_FLAGS_RELEASE_INIT "${CMAKE_C_FLAGS} -O3 -g0")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/inc/*.h)

# include directories
include_directories(${PROJECT_SOURCE_DIR}/inc/)

# building library
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
