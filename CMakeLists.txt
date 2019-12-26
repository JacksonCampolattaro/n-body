

# Sets minimum CMake version
cmake_minimum_required(VERSION 3.4)

# Add pkgConfig to more easily add GTKMM
find_package(PkgConfig REQUIRED)

# Add Hunter to more easily add Magnum
include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.16.15.tar.gz"
        SHA1 "6974c2150fc0d3b09de3ad1efcbf15d360647ffa"
)

# Sets project name
project(n_body)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

# Libraries
########################################################################################################################

# SDL2
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

# Corrade
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON) # What does this do?
hunter_add_package(corrade)
find_package(Corrade CONFIG REQUIRED COMPONENTS
        Containers
        Interconnect
        PluginManager
        TestSuite
        Utility
        )

# Magnum
hunter_add_package(magnum)
find_package(Magnum CONFIG REQUIRED COMPONENTS
        GL
        MeshTools
        Primitives
        Shaders
        Sdl2Application
        )

# GTKMM
pkg_check_modules(GTKMM gtkmm-3.0)

########################################################################################################################

# Adds the main subdirectory
add_subdirectory(src)