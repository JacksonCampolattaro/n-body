//
// Created by Jackson Campolattaro on 7/29/23.
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
#include "Benchmark/Graders/ConstitutionalGrader.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);
    Glib::init();

    std::size_t iterations = 1;

    //json scenario = Generator::trio();
    //json scenario = Generator::createScenario(Generator::uniformRandomVolume, 1000);
    //NBody::ConstitutionalGrader grader{scenario};
    ConstitutionalGrader grader{std::filesystem::path{"../n-body-scenarios/benchmark/LOW.bin"}};

    Simulation simulation = grader.scenario();
    Gravity rule = grader.rule();

    QuadrupoleBarnesHutSolver<Gravity> solver{simulation, rule};
    solver.theta() = 0.6;

    spdlog::info("Running {} for 1 iteration", solver.name());
    solver.step();
    auto error = grader.error(simulation);
    spdlog::info("Error = {} %", error);
}
