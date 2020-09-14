
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

        Model/Solver/Solver.h Model/Solver/Naive.cpp Model/Solver/Naive.h)

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
