
find_package(Catch2 3.1.0 REQUIRED)

add_executable(test_Physics
        gravity.cpp symmetricTensor.cpp multipole.cpp boundingBox.cpp)

target_link_libraries(test_Physics PRIVATE
        Catch2::Catch2WithMain
        NBody::Physics
        NBody::Simulation)

#include(Catch)
#catch_discover_tests(test_Physics)
