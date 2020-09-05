

# Set minimum CMake version
cmake_minimum_required(VERSION 3.12)

# Set project name
project(n_body)

# Set C++ version
set(CMAKE_CXX_STANDARD 17)

# Add Gtkmm's include files
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Add spdlog's include files
find_package(spdlog REQUIRED)

# Add Magnum's include files
find_package(Magnum REQUIRED
        GL
        MeshTools
        Primitives
        Shaders
        )

if (MAGNUM_TARGET_GLES AND NOT MAGNUM_TARGET_DESKTOP_GLES)
    find_package(Magnum REQUIRED EglContext)
elseif (CORRADE_TARGET_WINDOWS)
    find_package(Magnum REQUIRED WglContext)
elseif (CORRADE_TARGET_APPLE)
    find_package(Magnum REQUIRED CglContext)
elseif (CORRADE_TARGET_UNIX)
    find_package(Magnum REQUIRED GlxContext)
else ()
    message(FATAL_ERROR "Magnum context creation is not supported on this platform")
endif ()
#set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Add main subdirectory
add_subdirectory(src)

# Add test subdirectory
#add_subdirectory(test)

# Add Doxygen target
find_package(Doxygen REQUIRED)
add_custom_target(documentation
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generating documentation"
    )