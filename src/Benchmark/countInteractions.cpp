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

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    Glib::init();

    //json scenario = Generator::realisticGalaxy();
    //json scenario = Generator::trio();
    json scenario = Generator::createScenario(Generator::uniformRandomVolume, 112'500);

    Simulation simulation;
    from_json(scenario, simulation);
    SimpleTrackingRule<Gravity> rule{};

    //QuadrupoleBarnesHutSolver<SimpleTrackingRule<Gravity>> solver{simulation, rule};

    //QuadrupoleLinearBVHSolver<SimpleTrackingRule<Gravity>> solver{simulation, rule};
    //solver.theta() = 0.7925781;

    //QuadrupoleFMMSolver<SimpleTrackingRule<Gravity>> solver{simulation, rule};
    //QuadrupoleImplicitFMMSolver<SimpleTrackingRule<Gravity>> solver{simulation, rule};
    //solver.theta() = 0.38476565;

    //QuadrupoleMVDRSolver<SimpleTrackingRule<Gravity>> solver{simulation, rule};
    //QuadrupoleImplicitMVDRSolver<SimpleTrackingRule<Gravity>> solver{simulation, rule};
    //solver.theta() = 0.7925781;

    QuadrupoleBarnesHutSolver<SimpleTrackingRule<Gravity>> solver{simulation, rule};
    solver.descentCriterion().theta() = 0.5;

    spdlog::info("Running {} for 1 iteration", solver.name());
    solver.step();

    std::cout << rule
              << "Approximation ratio: "
              << ((float) rule.totalCount() / (float) (simulation.particleCount() * simulation.particleCount()))
              << std::endl;

}
