
# Add Gtkmm's include files
find_package(Gtkmm)

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

# Enable openmp
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Set files to be compiled
add_executable(${CMAKE_PROJECT_NAME}

        main.cpp

        Controller/Logger.cpp Controller/Logger.h
        Controller/Application.cpp Controller/Application.h

        Model/Simulation.cpp Model/Simulation.h
        Model/Entity.cpp Model/Entity.cpp
        Model/Position.h
        Model/Velocity.h

        Model/Drawable/Drawable.cpp Model/Drawable/Drawable.h
        Model/Drawable/Color.h
        Model/Physics/Physics.h
        Model/Physics/Rule.cpp Model/Physics/Rule.h
        Model/Physics/PassiveElement.cpp Model/Physics/PassiveElement.h
        Model/Physics/ActiveElement.cpp Model/Physics/ActiveElement.h

        View/View.h
        View/Painter.cpp View/Painter.h
        View/Interactive.cpp View/Interactive.h

        Model/Solver/Solver.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        ${GTKMM_LIBRARIES}
        Magnum::GLContext
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders
        spdlog::spdlog
        OpenMP::OpenMP_CXX
        )
