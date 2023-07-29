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
#include <NBody/Simulation/Solvers/ImplicitLinearBVHFMMSolver.h>

#include "../Generator.h"
#include "../bestTheta.h"
#include "../benchmark.h"

template<typename S>
void runFastTests(const std::string &label, std::ostream &out) {

    // First, determine the best value of theta for this solver
    ConstitutionalGrader lowGrader{std::filesystem::path{"../n-body-scenarios/benchmark/LOW.bin"}};
    auto optimalTheta = lowGrader.optimalTheta<S>();
    spdlog::info("Using theta = {}", optimalTheta);

    // Next, time it for several simulations
    for (const std::string &dataset: {"LOW", "MED"/*, "HI"*/}) {

        Physics::Gravity rule{};
        Simulation scenario;
        std::ifstream scenarioFile{fmt::format("../n-body-scenarios/benchmark/{}.bin", dataset)};
        assert(scenarioFile.is_open());
        from_tipsy(scenarioFile, scenario);
        spdlog::info("loaded {} particles", scenario.particleCount());

        S solver{scenario, rule};
        solver.descentCriterion().theta() = optimalTheta;
        auto time = timedStep(solver);
        spdlog::debug("{}s", time.count());
        out << label << ","
            << scenario.particleCount() << ","
            << optimalTheta << ","
            << time.count() << "\n";
    }
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);

    std::ofstream out{"benchmarks/all-solvers-agora-data.csv"};
    out << "Solver,N,Theta,Time\n";

    runFastTests<QuadrupoleBarnesHutSolver<Gravity>>("BH",  out);
    //runFastTests<QuadrupoleImplicitReverseBarnesHutSolver<Gravity>>("RBH",  out);
    //runFastTests<QuadrupoleLinearBVHSolver<Gravity>>("LBVH-BH",  out);
    //runFastTests<QuadrupoleImplicitFMMSolver<Gravity>>("FMM",  out);
    //runFastTests<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR-4p",  out);
    //runFastTests<OctupoleImplicitMVDRSolver<Gravity>>("MVDR-8p",  out);
    //runFastTests<QuadrupoleImplicitLinearBVHFMMSolver<Gravity>>("LBVH-FMM",  out);

}
