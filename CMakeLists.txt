

# Set minimum CMake version
cmake_minimum_required(VERSION 3.14)

# Set project name
project(n_body)

# Set C++ version
set(CMAKE_CXX_STANDARD 17)

# Make our modules available
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

# Include our targets
add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(test)
