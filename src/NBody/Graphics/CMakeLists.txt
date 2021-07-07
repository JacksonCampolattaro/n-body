
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

add_library(Graphics
        Color.h Color.cpp
        Sphere.h Sphere.cpp)

target_include_directories(Graphics PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Graphics PUBLIC

        Magnum::Magnum
        Magnum::GL
        Magnum::MeshTools
        Magnum::Primitives

        nlohmann_json::nlohmann_json
        )

add_library(NBody::Graphics ALIAS Graphics)
