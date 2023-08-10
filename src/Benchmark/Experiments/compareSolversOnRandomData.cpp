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
    spdlog::set_level(spdlog::level::info);

    std::ofstream out{argc > 1 ? std::string{argv[1]} : "benchmarks/all-solvers-random-data.csv"};
    out << "Solver,Multipole Order,N,Theta,Time,"
           "Particle-Particle,Particle-Node,Node-Particle,Node-Node,"
           "Approximation Ratio\n";

    std::size_t nMax = 250'000;

    for (std::size_t n = 15'000; n < nMax; n = n * 1.15) {

        spdlog::info("Generating a random dataset with {} particles", n);
        Simulation scenario = Generator::createScenario(Generator::uniformRandomVolume, n);
        Gravity rule{};

        ConstitutionalGrader grader{scenario, rule};

        runTest<QuadrupoleLinearBVHSolver<Gravity>>("LBVH,Quadrupole", grader, out);
        runTest<QuadrupoleBarnesHutSolver<Gravity>>("BH,Quadrupole", grader, out);
        runTest<QuadrupoleImplicitFMMSolver<Gravity>>("FMM,Quadrupole", grader, out);
        runTest<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR,Quadrupole", grader, out);

        runTest<OctupoleLinearBVHSolver<Gravity>>("LBVH,Octupole", grader, out);
        runTest<OctupoleBarnesHutSolver<Gravity>>("BH,Octupole", grader, out);
        runTest<OctupoleImplicitFMMSolver<Gravity>>("FMM,Octupole", grader, out);
        runTest<OctupoleImplicitMVDRSolver<Gravity>>("MVDR,Octupole", grader, out);

        runTest<HexadecupoleLinearBVHSolver<Gravity>>("LBVH,Hexadecupole", grader, out);
        runTest<HexadecupoleBarnesHutSolver<Gravity>>("BH,Hexadecupole", grader, out);
        runTest<HexadecupoleImplicitFMMSolver<Gravity>>("FMM,Hexadecupole", grader, out);
        runTest<HexadecupoleImplicitMVDRSolver<Gravity>>("MVDR,Hexadecupole", grader, out);

    }

}
