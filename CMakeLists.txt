

# Sets minimum CMake version
cmake_minimum_required(VERSION 3.4)

# Add pkgConfig to more easily add GTKMM
find_package(PkgConfig REQUIRED)

# Add Hunter to more easily add Magnum
include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
        SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
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
hunter_add_package(corrade)
find_package(Corrade CONFIG REQUIRED COMPONENTS
        Containers
        Interconnect
        PluginManager
        TestSuite
        Utility
        )
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON) # What does this do?

# Magnum
hunter_add_package(magnum)
find_package(Magnum CONFIG REQUIRED
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