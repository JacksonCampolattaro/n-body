//
// Created by Jackson Campolattaro on 5/29/23.
//

#include <gtkmm.h>
#include <spdlog/spdlog.h>

#include <NBody/Physics/Rules/Gravity.h>
#include <NBody/Physics/Rules/SimpleTrackingRule.h>
#include <NBody/Physics/Rules/AdvancedTrackingRule.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/ReverseBarnesHutSolver.h>
#include <NBody/Simulation/Solvers/ImplicitReverseBarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/MVDRSolver.h>
#include <NBody/Simulation/Solvers/ImplicitMVDRSolver.h>
#include <NBody/Simulation/Solvers/OctreeDualTraversalSolver.h>
#include <NBody/Simulation/Solvers/FMMSolver.h>
#include <NBody/Simulation/Solvers/ImplicitFMMSolver.h>

#include "Generator.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    Glib::init();

    std::size_t iterations = 1;

    json scenario = Generator::realisticGalaxy();
    //json scenario = Generator::trio();
    //json scenario = Generator::createScenario(Generator::uniformRandomVolume, 10'000);

    Simulation simulation;
    from_json(scenario, simulation);
    Gravity rule{};

    QuadrupoleImplicitMVDRSolver<Gravity> solver{simulation, rule};
    solver.theta() = 0.6;

    spdlog::info("Running {} for {} iterations",
                 solver.name(), iterations);
    for (int i = 0; i < iterations; ++i) {
        solver.step();
    }
}
