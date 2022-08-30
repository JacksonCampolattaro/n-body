
find_package(Magnum REQUIRED
        GL
        MeshTools
        Primitives
        Shaders
        DebugTools
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

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtkmm REQUIRED IMPORTED_TARGET gtkmm-4.0 glibmm-2.68 giomm-2.68)
pkg_check_modules(adwaita REQUIRED IMPORTED_TARGET libadwaita-1)
pkg_check_modules(avcodec REQUIRED IMPORTED_TARGET
        libavdevice
        libavformat
        libavfilter
        libavcodec
        libswresample
        libswscale
        libavutil
        )
MESSAGE(STATUS "@@@@@@@@@@@@@@@@@ " ${avcodec_LIBRARIES})
MESSAGE(STATUS "@@@@@@@@@@@@@@@@@ " ${avcodec_LIBRARY_DIRS})
MESSAGE(STATUS "@@@@@@@@@@@@@@@@@ " ${avcodec_LDFLAGS})
MESSAGE(STATUS "@@@@@@@@@@@@@@@@@ " ${avcodec_INCLUDE_DIRS})
MESSAGE(STATUS "@@@@@@@@@@@@@@@@@ " ${avcodec_CFLAGS})
MESSAGE(STATUS "@@@@@@@@@@@@@@@@@ " ${avcodec_VERSION})
MESSAGE(STATUS "@@@@@@@@@@@@@@@@@ " ${avcodec_PREFIX})
MESSAGE(STATUS "@@@@@@@@@@@@@@@@@ " ${avcodec_INCLUDEDIR})
MESSAGE(STATUS "@@@@@@@@@@@@@@@@@ " ${avcodec_LIBDIR})

find_package(spdlog REQUIRED)

add_library(View
        ArcBall.cpp ArcBall.h
        InteractiveView.cpp InteractiveView.h
        SimpleView.cpp SimpleView.h
        Renderer.h
        Renderers/MultiRenderer.h
        Renderers/InstancedPhongRenderer.cpp Renderers/InstancedPhongRenderer.h
        Renderers/PhongRenderer.cpp Renderers/PhongRenderer.h
        Renderers/FlatRenderer.cpp Renderers/FlatRenderer.h
        Renderers/InstancedFlatRenderer.cpp Renderers/InstancedFlatRenderer.h
        Camera.h
        ArcBallControllableCamera.cpp ArcBallControllableCamera.h
        Recorder.cpp Recorder.h
        View.h VideoOutputStream.cpp VideoOutputStream.h)

target_include_directories(View PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(View PUBLIC

        NBody::Simulation

        spdlog::spdlog

        PkgConfig::gtkmm
        PkgConfig::adwaita

        Magnum::Magnum
        Magnum::GLContext
        Magnum::GL
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders
        Magnum::DebugTools

        PkgConfig::avcodec
        )


#target_compile_options(View PRIVATE
#        -Werror
#        -Wall
#        -Wextra)

add_library(NBody::View ALIAS View)