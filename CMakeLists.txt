

# Set minimum CMake version
cmake_minimum_required(VERSION 3.12)

# Set project name
project(n_body)

# Add main subdirectory
add_subdirectory(src)

# Add Doxygen target
find_package(Doxygen)
add_custom_target(doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generating documentation"
    )