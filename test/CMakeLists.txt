
# Set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall --coverage")

add_executable(test

    ../src/Model/Simulation.h ../src/Model/Simulation.cpp
    ../src/Model/Position.h ../src/Model/Position.cpp
    ../src/Model/Velocity.h ../src/Model/Velocity.cpp

    test.cpp
    testSimulation.cpp
    )
