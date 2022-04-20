
find_package(nlohmann_json REQUIRED)
find_package(SigC++ REQUIRED)
find_package(spdlog REQUIRED)
find_package(TBB REQUIRED)

include(FetchContent)
FetchContent_Declare(entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.8.1)
FetchContent_MakeAvailable(entt)

add_library(Simulation
        Simulation.h Simulation.cpp
        Solver.h
        Solvers/NaiveSolver.h Solvers/NaiveSolver.cpp
        )

target_include_directories(Simulation PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Simulation PUBLIC

        NBody::Physics
        NBody::Graphics

        EnTT::EnTT
        nlohmann_json::nlohmann_json

        spdlog::spdlog
        ${SIGCPP_LIBRARIES}
        TBB::tbb
        )

add_library(NBody::Simulation ALIAS Simulation)

add_subdirectory(test)
