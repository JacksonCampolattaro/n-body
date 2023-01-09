
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(TBB REQUIRED)

find_package(PkgConfig)
pkg_check_modules(sigc++ REQUIRED IMPORTED_TARGET sigc++-3.0)

include(FetchContent)
FetchContent_Declare(entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.8.1)
FetchContent_MakeAvailable(entt)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtkmm REQUIRED IMPORTED_TARGET gtkmm-4.0 glibmm-2.68 giomm-2.68)

add_library(Simulation
        Simulation.h Simulation.cpp
        Solver.h
        Solvers/Tree.h
        Solvers/MultiSolver.h
        Solvers/NaiveSolver.h Solvers/NaiveSolver.cpp
        Solvers/BarnesHutSolver.cpp Solvers/BarnesHutSolver.h
        Solvers/Octree.cpp Solvers/Octree.h Runner.h
        Runners/OneStepRunner.cpp Runners/OneStepRunner.h Runners/NStepsRunner.cpp Runners/NStepsRunner.h Runners/ContinuousRunner.cpp Runners/ContinuousRunner.h Runners/MultiRunner.h )

target_include_directories(Simulation PUBLIC
        ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Simulation PUBLIC

        NBody::Physics
        NBody::Graphics

        EnTT::EnTT
        nlohmann_json::nlohmann_json

        PkgConfig::gtkmm
        PkgConfig::sigc++

        spdlog::spdlog
        TBB::tbb
        )

add_library(NBody::Simulation ALIAS Simulation )

add_subdirectory(test)
