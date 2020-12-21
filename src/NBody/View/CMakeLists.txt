# Add spdlog's include files
find_package(spdlog REQUIRED)

# Add rapidjson
find_package(RapidJSON REQUIRED)


# Add Gtkmm's include files
find_package(Gtkmm REQUIRED)

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

add_library(View

        View.h
        )

target_include_directories(View PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(View PUBLIC

        ${GTKMM_LIBRARIES}

        spdlog::spdlog

        Magnum::GLContext
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders
        )