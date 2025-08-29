cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Pathfinder VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/AStarPlanner.cpp
    src/ObstacleModel.cpp
    src/PathOptimizer.cpp
    src/RouteNodes.cpp
    src/main.cpp
)

# Create executable
add_executable(pathfinder ${SOURCES})

# Compiler-specific options
if(MSVC)
    target_compile_options(pathfinder PRIVATE /W4)
else()
    target_compile_options(pathfinder PRIVATE -Wall -Wextra -pedantic)
endif()

# Set output directory for executable
set_target_properties(pathfinder PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)