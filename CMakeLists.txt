

# Sets minimum CMake version
cmake_minimum_required(VERSION 3.12)


# Sets project name
project(n_body)

find_package(PkgConfig REQUIRED)


# GTKMM
########################################################################################################################
pkg_check_modules(GTKMM gtkmm-3.0)


# Raylib
########################################################################################################################

# Set this to the minimal version you want to support
find_package(raylib 2.5 QUIET) # Let CMake search for a raylib-config.cmake

# You could change the QUIET above to REQUIRED and remove this if() clause
# This part downloads raylib and builds it if it's not installed on your system
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)

    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

        # build raylib
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

    endif()

endif()



# Adds the main subdirectory
add_subdirectory(src)