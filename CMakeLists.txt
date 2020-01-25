

# Set minimum CMake version
cmake_minimum_required(VERSION 3.12)

# Set project name
project(n_body)

# Add main subdirectory
add_subdirectory(src)

# Doxygen
find_package(Doxygen)
add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ../Doxyfile
    )