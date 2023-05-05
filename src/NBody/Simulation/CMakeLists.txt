
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(TBB REQUIRED)

find_package(PkgConfig)
pkg_check_modules(sigc++ REQUIRED IMPORTED_TARGET sigc++-3.0)

include(FetchContent)
FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.8.1) # todo: update to the newest version of entt
set(ENTT_BUILD_TESTING OFF)
FetchContent_MakeAvailable(entt)


find_package(PkgConfig REQUIRED)
pkg_check_modules(gtkmm REQUIRED IMPORTED_TARGET gtkmm-4.0 glibmm-2.68 giomm-2.68)

add_library(Simulation
        Simulation.h Simulation.cpp
        Solver.h
        Solvers/MultiSolver.h
        Solvers/MortonSort.h
        Solvers/NaiveSolver.h Solvers/NaiveSolver.cpp
        Solvers/BarnesHutSolver.h
        Solvers/Trees/NodeType.h
        Solvers/Trees/NodeBase.h
        Solvers/Trees/Tree.h
        Solvers/Descent/DescentCriterionType.h
        Solvers/Trees/Octree.h
        Solvers/Trees/LinearBVH.h
        Runner.h
        Runners/MultiRunner.h
        Runners/OneStepRunner.cpp Runners/OneStepRunner.h
        Runners/NStepsRunner.cpp Runners/NStepsRunner.h
        Runners/ContinuousRunner.cpp Runners/ContinuousRunner.h
        Solvers/LinearBVHSolver.h
        Solvers/DualTreeSolver.h
        Solvers/DualTraversalSolver.h Solvers/MVDRSolver.h
        Solvers/ActiveTreeSolver.h Solvers/OctreeDualTraversalSolver.h
        Solvers/FMMSolver.h Solvers/Descent/activeTree.h
        Solvers/Descent/balancedLockstepDualTree.h Solvers/Descent/lockstepDualTree.h Solvers/Descent/collapseAccelerations.h Solvers/Descent/DescentCriterion/SideLengthOverDistance.h Solvers/Descent/DescentCriterion/DiagonalOverDistance.h Solvers/Descent/DescentCriterion/ProjectedDiagonalOverDistance.h Solvers/Descent/passiveTree.h Solvers/Descent/adaptiveDualTree.h Solvers/Descent/none.h Solvers/PassiveTreeSolver.h Solvers/ReverseBarnesHutSolver.h Solvers/Descent/adaptiveDualTreeImplicitField.h Solvers/Descent/passiveTreeImplicitField.h Solvers/ImplicitReverseBarnesHutSolver.h Solvers/ImplicitPassiveTreeSolver.h Solvers/ImplicitMVDRSolver.h Solvers/ImplicitDualTreeSolver.h Solvers/Descent/lockstepDualTreeImplicitField.h Solvers/Descent/balancedLockstepDualTreeImplicitField.h Solvers/ImplicitFMMSolver.h Solvers/ImplicitDualTraversalSolver.h)

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

#add_subdirectory(test)
