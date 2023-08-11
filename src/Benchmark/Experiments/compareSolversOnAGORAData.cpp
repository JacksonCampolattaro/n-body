//
// Created by Jackson Campolattaro on 6/1/23.
//
#include <spdlog/spdlog.h>

#include <boost/program_options.hpp>

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
    RMSGrader lowGrader{std::filesystem::path{"../n-body-scenarios/benchmark/LOW.bin"}};
    auto optimalTheta = lowGrader.optimalTheta<S>();
    spdlog::info("Using theta = {}", optimalTheta);

    // Next, time it for several simulations
    for (const std::string &dataset: {"LOW", "MED", "HI"}) {

        Physics::Gravity rule{};
        Simulation scenario;
        std::ifstream scenarioFile{fmt::format("../n-body-scenarios/benchmark/{}.bin", dataset)};
        assert(scenarioFile.is_open());
        from_tipsy(scenarioFile, scenario);
        spdlog::info("Loaded {} particles", scenario.particleCount());

        S solver{scenario, rule};
        solver.descentCriterion().theta() = optimalTheta;
        auto time = timedStep(solver);
        spdlog::info("{} --> {}s", solver.name(), time.count());
        out << label << ","
            << scenario.particleCount() << ","
            << optimalTheta << ","
            << time.count() << "\n" << std::flush;

        // If any scenario takes too long to run, stop early!
        if (time.count() > 25.0f) break;
    }
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    std::ofstream out{argc > 1 ? std::string{argv[1]} : "benchmarks/agora/all-solvers-rms.csv"};
    out << "Solver,Multipole Order,N,Theta,Time\n";

    runFastTests<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR,Quadrupole",  out);
    runFastTests<OctupoleImplicitMVDRSolver<Gravity>>("MVDR,Octupole", out);
    //runFastTests<HexadecupoleImplicitMVDRSolver<Gravity>>("MVDR,Hexadecupole", out);

    runFastTests<QuadrupoleImplicitFMMSolver<Gravity>>("FMM,Quadrupole",  out);
    runFastTests<OctupoleImplicitFMMSolver<Gravity>>("FMM,Octupole",  out);
    //runFastTests<HexadecupoleImplicitFMMSolver<Gravity>>("FMM,Hexadecupole",  out);

    runFastTests<QuadrupoleLinearBVHSolver<Gravity>>("LBVH,Quadrupole",  out);
    //runFastTests<OctupoleLinearBVHSolver<Gravity>>("LBVH,Octupole",  out);
    //runFastTests<HexadecupoleLinearBVHSolver<Gravity>>("LBVH,Hexadecupole",  out);

    runFastTests<QuadrupoleBarnesHutSolver<Gravity>>("BH,Quadrupole",  out);
    //runFastTests<OctupoleBarnesHutSolver<Gravity>>("BH,Octupole",  out);
    //runFastTests<HexadecupoleBarnesHutSolver<Gravity>>("BH,Hexadecupole",  out);

}
