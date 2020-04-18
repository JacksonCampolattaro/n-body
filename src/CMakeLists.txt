
# Set C++ version
set(CMAKE_CXX_STANDARD 17)

# Add Gtkmm's include files
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Add Magnum's include files
find_package(Magnum REQUIRED
    GL
    SceneGraph
    MeshTools
    Shaders
    )

if(MAGNUM_TARGET_GLES AND NOT MAGNUM_TARGET_DESKTOP_GLES)
    find_package(Magnum REQUIRED EglContext)
elseif(CORRADE_TARGET_WINDOWS)
    find_package(Magnum REQUIRED WglContext)
elseif(CORRADE_TARGET_APPLE)
    find_package(Magnum REQUIRED CglContext)
elseif(CORRADE_TARGET_UNIX)
    find_package(Magnum REQUIRED GlxContext)
else()
    message(FATAL_ERROR "Magnum context creation is not supported on this platform")
endif()
#set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Set files to be compiled
add_executable(${CMAKE_PROJECT_NAME}
    main.cpp

    Controller/Controller.h
    Controller/Logger.cpp Controller/Logger.h
    Controller/Application.cpp Controller/Application.h

    Model/Model.h
    Model/Entity.cpp Model/Entity.h
    Model/Simulation.cpp Model/Simulation.h
    Model/Position.cpp Model/Position.h
    Model/Velocity.cpp Model/Velocity.h

    View/Builder.cpp View/Builder.h
    View/MagnumViewport.cpp View/MagnumViewport.h
    View/ViewerWindow.cpp View/ViewerWindow.h
    )

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    ${GTKMM_LIBRARIES}
    Magnum::GLContext
    Magnum::GL
    Magnum::Magnum
    Magnum::SceneGraph
    Magnum::MeshTools
    Magnum::Shaders
    )
