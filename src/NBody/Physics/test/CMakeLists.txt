
find_package(Catch2 REQUIRED)

add_executable(test_Physics main.cpp
        position.cpp
        velocity.cpp
        active.cpp
        gravity.cpp)

target_link_libraries(test_Physics PRIVATE
        NBody::Physics
        Catch2::Catch2)

add_test(NAME TestPhysics COMMAND test_Physics)
