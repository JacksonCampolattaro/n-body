

# Sets minimum CMake version
cmake_minimum_required(VERSION 3.4)

# Sets project name
project(n_body)

add_subdirectory(corrade EXCLUDE_FROM_ALL)
set(WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
add_subdirectory(magnum EXCLUDE_FROM_ALL)

include(FindPkgConfig)
pkg_check_modules(SIGC REQUIRED sigc++-2.0)

# Adds the main subdirectory
add_subdirectory(src)