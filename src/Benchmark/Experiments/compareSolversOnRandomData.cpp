//
// Created by Jackson Campolattaro on 5/31/23.
//
#include <gtkmm.h>
#include <spdlog/spdlog.h>

#include <NBody/Physics/Rules/Gravity.h>
#include <NBody/Physics/Rules/SimpleTrackingRule.h>

#include <NBody/Simulation/Simulation.h>

#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/ImplicitReverseBarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/ImplicitFMMSolver.h>
#include <NBody/Simulation/Solvers/ImplicitMVDRSolver.h>

#include "../Generator.h"
#include "../bestTheta.h"
#include "../benchmark.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);

    std::ofstream out{"benchmarks/all-solvers-random-data.csv"};
    out << "Solver,N,Theta,Time,"
           "Particle-Particle,Particle-Node,Node-Particle,Node-Node,"
           "Approximation Ratio\n";

    std::size_t nMax = 100'000;

    for (std::size_t n = 100; n < nMax; n = n * 1.5) {

        spdlog::info("Generating a random dataset with {} particles", n);
        Simulation scenario = Generator::createScenario(Generator::uniformRandomVolume, n);
        Gravity rule{};

        ConstitutionalGrader grader{scenario, rule};

        runTest<QuadrupoleBarnesHutSolver<Gravity>>("BH", grader, out);
        //runTest<QuadrupoleImplicitReverseBarnesHutSolver<Gravity>>("RBH", grader, out);
        runTest<QuadrupoleLinearBVHSolver<Gravity>>("LBVH", grader, out);
        runTest<QuadrupoleImplicitFMMSolver<Gravity>>("FMM", grader, out);
        runTest<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR", grader, out);

    }

}
