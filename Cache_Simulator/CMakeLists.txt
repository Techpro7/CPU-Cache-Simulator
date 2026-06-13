cmake_minimum_required(VERSION 3.15)

project(CacheSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header files
include_directories(include)

# Collect all cpp files from src/
file(GLOB SRC_FILES
    src/*.cpp
)

# Main simulator executable
add_executable(
    cache_simulator
    ${SRC_FILES}
)

# Trace generator executable
add_executable(
    generate_trace
    tools/generate_trace.cpp
)