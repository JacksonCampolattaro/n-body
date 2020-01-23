

# Set minimum CMake version
cmake_minimum_required(VERSION 3.12)

# Set project name
project(n_body)

# Sets Gtkmm path information
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

# Add main subdirectory
add_subdirectory(src)