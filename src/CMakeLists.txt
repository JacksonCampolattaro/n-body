
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

add_subdirectory(Model)

# Set files to be compiled
add_executable(${CMAKE_PROJECT_NAME}

        main.cpp

        Controller/Logger.cpp Controller/Logger.h
        Controller/Application.cpp Controller/Application.h

        View/View.h
        View/Painter.cpp View/Painter.h
        View/Interactive.cpp View/Interactive.h

        Model/Solver/Solver.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE

        Model

        ${GTKMM_LIBRARIES}

        Magnum::GLContext
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders

        )
