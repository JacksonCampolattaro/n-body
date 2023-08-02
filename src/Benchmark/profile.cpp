//
// Created by Jackson Campolattaro on 5/29/23.
//

#include <gtkmm.h>
#include <spdlog/spdlog.h>

#include "NBody/Physics/Rules/Gravity.h"
#include "NBody/Physics/Rules/SimpleTrackingRule.h"
#include "NBody/Physics/Rules/AdvancedTrackingRule.h"

#include "NBody/Simulation/Simulation.h"
#include "NBody/Simulation/Solvers/NaiveSolver.h"
#include "NBody/Simulation/Solvers/BarnesHutSolver.h"
#include "NBody/Simulation/Solvers/ReverseBarnesHutSolver.h"
#include "NBody/Simulation/Solvers/ImplicitReverseBarnesHutSolver.h"
#include "NBody/Simulation/Solvers/LinearBVHSolver.h"
#include "NBody/Simulation/Solvers/MVDRSolver.h"
#include "NBody/Simulation/Solvers/ImplicitMVDRSolver.h"
#include "NBody/Simulation/Solvers/OctreeDualTraversalSolver.h"
#include "NBody/Simulation/Solvers/FMMSolver.h"
#include "NBody/Simulation/Solvers/ImplicitFMMSolver.h"

#include "Benchmark/Generator.h"
#include "benchmark.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);
    Glib::init();

    std::size_t iterations = 1;

    Simulation simulation = Generator::fromFile("../n-body-scenarios/benchmark/MED.bin");
    Gravity rule{};

    QuadrupoleLinearBVHSolver<Gravity> solver{simulation, rule};
    solver.theta() = 0.8;

    spdlog::info("Running {} for {} iterations",
                 solver.name(), iterations);
    for (int i = 0; i < iterations; ++i) {
        auto time = timedInvoke([&]() {
            solver.step();
        });
        spdlog::info("Time = {} s", time.count());
    }
}
