

# Sets minimum CMake version
cmake_minimum_required(VERSION 3.12)


# Sets project name
project(n_body)

find_package(PkgConfig REQUIRED)

# Enable adding modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

# Libraries
########################################################################################################################

# Add Corrade as a subproject
add_subdirectory(libs/corrade EXCLUDE_FROM_ALL)

# Add Magnum as a subproject, enable Sdl2Application
set(WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
add_subdirectory(libs/magnum EXCLUDE_FROM_ALL)

# GTKMM
pkg_check_modules(GTKMM gtkmm-3.0)

########################################################################################################################


# Adds the main subdirectory
add_subdirectory(src)