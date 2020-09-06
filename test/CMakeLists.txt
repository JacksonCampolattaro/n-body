
add_executable(test_position

        ../src/Model/Position.h

        testPosition.cpp)

add_executable(test_velocity

        ../src/Model/Velocity.h

        testVelocity.cpp)

add_executable(test_color

        ../src/Model/Drawable/Color.h

        testColor.cpp)

add_executable(test_drawable

        ../src/Model/Drawable/Drawable.h ../src/Model/Drawable/Drawable.cpp

        testDrawable.cpp)

add_executable(test_passiveElement

        ../src/Model/Physics/PassiveElement.h ../src/Model/Physics/PassiveElement.cpp

        testPassiveElement.cpp)

add_executable(test_activeElement

        ../src/Model/Physics/ActiveElement.h ../src/Model/Physics/ActiveElement.cpp

        testActiveElement.cpp)

add_executable(test_entity

        ../src/Model/Simulation.cpp ../src/Model/Simulation.h
        ../src/Model/Entity.cpp ../src/Model/Entity.cpp
        ../src/Model/Position.h
        ../src/Model/Velocity.h
        ../src/Model/Drawable/Drawable.cpp ../src/Model/Drawable/Drawable.h
        ../src/Model/Physics/Physics.h
        ../src/Model/Physics/Rule.cpp ../src/Model/Physics/Rule.h
        ../src/Model/Physics/PassiveElement.cpp ../src/Model/Physics/PassiveElement.h
        ../src/Model/Physics/ActiveElement.cpp ../src/Model/Physics/ActiveElement.h

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

        ../src/Model/Simulation.cpp ../src/Model/Simulation.h
        ../src/Model/Entity.cpp ../src/Model/Entity.cpp
        ../src/Model/Position.h
        ../src/Model/Velocity.h
        ../src/Model/Drawable/Drawable.cpp ../src/Model/Drawable/Drawable.h
        ../src/Model/Physics/Physics.h
        ../src/Model/Physics/Rule.cpp ../src/Model/Physics/Rule.h
        ../src/Model/Physics/PassiveElement.cpp ../src/Model/Physics/PassiveElement.h
        ../src/Model/Physics/ActiveElement.cpp ../src/Model/Physics/ActiveElement.h

        testSimulation.cpp)

target_link_libraries(test_simulation PRIVATE
        ${GTKMM_LIBRARIES}
        Magnum::GLContext
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders
        spdlog::spdlog
        )
