
add_executable(test_position

        ../src/Model/Position.h ../src/Model/Position.cpp

        testPosition.cpp)

add_executable(test_velocity

        ../src/Model/Velocity.h ../src/Model/Velocity.cpp

        testVelocity.cpp)

add_executable(test_drawable

        ../src/Model/Drawable/Drawable.h ../src/Model/Drawable/Drawable.cpp

        testDrawable.cpp)

add_executable(test_passiveElement

        ../src/Model/Physics/PassiveElement.h ../src/Model/Physics/PassiveElement.cpp

        testPassiveElement.cpp)
