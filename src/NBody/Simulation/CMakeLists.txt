
find_package(CGAL)

add_library(Simulation

        Simulation.h Simulation.cpp)

target_include_directories(Simulation PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Simulation PUBLIC
        spdlog::spdlog
        CGAL::CGAL)

add_library(NBody::Simulation ALIAS Simulation)