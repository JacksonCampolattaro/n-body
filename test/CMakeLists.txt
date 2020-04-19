
# Set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")

add_executable(test
    test.cpp
    testSimulation.cpp

    ../src/Model/Simulation.h ../src/Model/Simulation.cpp
    ../src/Model/Entity.h ../src/Model/Entity.cpp
    ../src/Model/Position.h ../src/Model/Position.cpp
    ../src/Model/Velocity.h ../src/Model/Velocity.cpp
    testEntity.cpp)
