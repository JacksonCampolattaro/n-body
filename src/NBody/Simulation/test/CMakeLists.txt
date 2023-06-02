
find_package(Catch2 3.1.0 REQUIRED)

add_executable(test_Simulation
        simulation.cpp)

target_link_libraries(test_Simulation PRIVATE
        Catch2::Catch2WithMain
        NBody::Simulation)
