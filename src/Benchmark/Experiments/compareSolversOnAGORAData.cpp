//
// Created by Jackson Campolattaro on 6/1/23.
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

    std::ofstream out{"benchmarks/all-solvers-agora-data.csv"};
    out << "Solver,N,Theta,Iteration,Time\n";

    for (const std::string &dataset: {"LOW", "MED", "HI"}) {

        ConstitutionalGrader grader{std::filesystem::path{
                fmt::format("../n-body-scenarios/benchmark/{}.bin", dataset)
        }};

        runFastTest<QuadrupoleBarnesHutSolver<Gravity>>("BH", grader, out);
        //runFastTest<QuadrupoleImplicitReverseBarnesHutSolver<Gravity>>("RBH", grader, out, 0.3);
        runFastTest<QuadrupoleLinearBVHSolver<Gravity>>("LBVH", grader, out);
        runFastTest<QuadrupoleImplicitFMMSolver<Gravity>>("FMM", grader, out);
        runFastTest<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR", grader, out);

    }

}
