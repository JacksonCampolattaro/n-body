
# Sets minimum CMake version
cmake_minimum_required(VERSION 3.12)

# Sets project name
project(n_body)

# Making sure Gtk's C++ bindings are loaded
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

find_package(raylib 2.0 QUIET) # Let CMake search for a raylib-config.cmake

# Adds the main subdirectory
add_subdirectory(src)