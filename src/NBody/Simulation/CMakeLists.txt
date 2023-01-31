
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
        BoundingBox.h
        Solver.h
        Solvers/MultiSolver.h
        Solvers/MortonSort.h
        Solvers/NaiveSolver.h Solvers/NaiveSolver.cpp
        Solvers/BarnesHutSolver.h
        Solvers/Trees/Tree.h
        Solvers/Trees/ActiveNode.h
        Solvers/Trees/PassiveNode.h
        Solvers/Trees/FieldTree.h
        Solvers/Trees/DescentCriterion.h
        Solvers/Trees/Octree.h
        Solvers/Trees/LinearBVH.h
        Solvers/Trees/PassiveOctree.h
        Runner.h
        Runners/MultiRunner.h
        Runners/OneStepRunner.cpp Runners/OneStepRunner.h
        Runners/NStepsRunner.cpp Runners/NStepsRunner.h
        Runners/ContinuousRunner.cpp Runners/ContinuousRunner.h
        Solvers/LinearBVHSolver.h
        Solvers/DualTreeSolver.h
        Solvers/Trees/DualNode.h Solvers/Trees/DualOctree.h Solvers/DualTraversalSolver.h)

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

add_library(NBody::Simulation ALIAS Simulation)

add_subdirectory(test)
