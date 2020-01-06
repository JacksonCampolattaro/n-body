

# Sets minimum CMake version
cmake_minimum_required(VERSION 3.4)

# Sets project name
project(n_body)

# Sets modules directory
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

include(FindPkgConfig)
pkg_check_modules(SIGC REQUIRED sigc++-2.0)

# Adds the main subdirectory
add_subdirectory(src)