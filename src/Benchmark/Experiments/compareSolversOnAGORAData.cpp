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
    Glib::init();

    std::ofstream out{"benchmarks/all-solvers-agora-data.csv"};
    out << "Solver,N,Theta,Iteration,Time\n";

    ConstitutionalGrader medGrader{std::filesystem::path{"../n-body-scenarios/benchmark/MED.bin"}};
    ConstitutionalGrader hiGrader{std::filesystem::path{"../n-body-scenarios/benchmark/HI.bin"}};

    for (const std::string &dataset: {"LOW", "MED", "HI"}) {

        ConstitutionalGrader grader{std::filesystem::path{
                fmt::format("../n-body-scenarios/benchmark/{}.bin", dataset)
        }};

        //        spdlog::info("Loading AGORA {} dataset", dataset);
        //        Simulation s;
        //        std::ifstream file{fmt::format("../n-body-scenarios/benchmark/{}.bin", dataset)};
        //        from_tipsy(file, s);
        //        json scenario;
        //        to_json(scenario, s);
        //
        //        Gravity rule{};
        //
        //        ConstitutionalGrader grader{scenario, rule};

        runShortTest<QuadrupoleBarnesHutSolver<Gravity>>("BH", grader, out);
        //runShortTest<QuadrupoleImplicitReverseBarnesHutSolver<Gravity>>("RBH", grader, out);
        runShortTest<QuadrupoleLinearBVHSolver<Gravity>>("LBVH", grader, out);
        runShortTest<QuadrupoleImplicitFMMSolver<Gravity>>("FMM", grader, out);
        runShortTest<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR", grader, out);

    }

}
