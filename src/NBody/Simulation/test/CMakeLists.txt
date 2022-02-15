
find_package(Catch2 REQUIRED)

add_executable(test_Simulation main.cpp
        simulation.cpp)

target_link_libraries(test_Simulation PRIVATE
        NBody::Simulation
        Catch2::Catch2)

add_test(NAME TestSimulation COMMAND test_Simulation)
