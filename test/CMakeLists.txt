
add_executable(test_position

        ../src/NBody/Model/Position.h

        testPosition.cpp)

add_executable(test_velocity

        ../src/NBody/Model/Velocity.h

        testVelocity.cpp)

add_executable(test_color

        ../src/NBody/Model/Drawable/Color.h

        testColor.cpp)

add_executable(test_drawable

        ../src/NBody/Model/Drawable/Drawable.h ../src/NBody/Model/Drawable/Drawable.cpp

        testDrawable.cpp)

add_executable(test_passiveElement

        ../src/NBody/Model/Physics/PassiveElement.h ../src/NBody/Model/Physics/PassiveElement.cpp

        testPassiveElement.cpp)

add_executable(test_activeElement

        ../src/NBody/Model/Physics/ActiveElement.h ../src/NBody/Model/Physics/ActiveElement.cpp

        testActiveElement.cpp)

add_executable(test_entity

        ../src/NBody/Model/Simulation.cpp ../src/NBody/Model/Simulation.h
        ../src/NBody/Model/Entity.cpp ../src/NBody/Model/Entity.cpp
        ../src/NBody/Model/Position.h
        ../src/NBody/Model/Velocity.h
        ../src/NBody/Model/Drawable/Drawable.cpp ../src/NBody/Model/Drawable/Drawable.h
        ../src/NBody/Model/Physics/Physics.h
        ../src/NBody/Model/Physics/Rule.cpp ../src/NBody/Model/Physics/Rule.h
        ../src/NBody/Model/Physics/PassiveElement.cpp ../src/NBody/Model/Physics/PassiveElement.h
        ../src/NBody/Model/Physics/ActiveElement.cpp ../src/NBody/Model/Physics/ActiveElement.h

        testEntity.cpp)

target_link_libraries(test_entity PRIVATE
        ${GTKMM_LIBRARIES}
        Magnum::GLContext
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders
        spdlog::spdlog
        )

add_executable(test_simulation

        ../src/NBody/Model/Simulation.cpp ../src/NBody/Model/Simulation.h
        ../src/NBody/Model/Entity.cpp ../src/NBody/Model/Entity.cpp
        ../src/NBody/Model/Position.h
        ../src/NBody/Model/Velocity.h
        ../src/NBody/Model/Drawable/Drawable.cpp ../src/NBody/Model/Drawable/Drawable.h
        ../src/NBody/Model/Physics/Physics.h
        ../src/NBody/Model/Physics/Rule.cpp ../src/NBody/Model/Physics/Rule.h
        ../src/NBody/Model/Physics/PassiveElement.cpp ../src/NBody/Model/Physics/PassiveElement.h
        ../src/NBody/Model/Physics/ActiveElement.cpp ../src/NBody/Model/Physics/ActiveElement.h

        testSimulation.cpp)

target_link_libraries(test_simulation PRIVATE
        Model

        ${GTKMM_LIBRARIES}

        Magnum::GLContext
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders

        spdlog::spdlog
        )

add_executable(bodies

        ../src/NBody/Model/Simulation.cpp ../src/NBody/Model/Simulation.h
        ../src/NBody/Model/Entity.cpp ../src/NBody/Model/Entity.cpp
        ../src/NBody/Model/Position.h
        ../src/NBody/Model/Velocity.h
        ../src/NBody/Model/Drawable/Drawable.cpp ../src/NBody/Model/Drawable/Drawable.h
        ../src/NBody/Model/Physics/Physics.h
        ../src/NBody/Model/Physics/Rule.cpp ../src/NBody/Model/Physics/Rule.h
        ../src/NBody/Model/Physics/PassiveElement.cpp ../src/NBody/Model/Physics/PassiveElement.h
        ../src/NBody/Model/Physics/ActiveElement.cpp ../src/NBody/Model/Physics/ActiveElement.h

        bodies.cpp)

target_link_libraries(bodies PRIVATE
        ${GTKMM_LIBRARIES}
        Magnum::GLContext
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders
        spdlog::spdlog
        )
