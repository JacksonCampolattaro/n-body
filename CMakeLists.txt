

# Sets minimum CMake version
cmake_minimum_required(VERSION 3.12)


# Sets project name
project(n_body)

find_package(PkgConfig REQUIRED)


# Libraries
########################################################################################################################

# Header-only
include_directories(libs)

# GTKMM
pkg_check_modules(GTKMM gtkmm-3.0)

########################################################################################################################


# Adds the main subdirectory
add_subdirectory(src)