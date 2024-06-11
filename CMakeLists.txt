cmake_minimum_required(VERSION 3.22.1)
project(gameOfLife)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2
find_package(SDL2 REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/grid.cpp
    src/simulation.cpp
)

# Add executable
add_executable(gameOfLife ${SOURCES})

# Link libraries
target_link_libraries(gameOfLife ${SDL2_LIBRARIES})

# Compiler flags
target_compile_options(gameOfLife PRIVATE -Wall -Wextra -pedantic)
