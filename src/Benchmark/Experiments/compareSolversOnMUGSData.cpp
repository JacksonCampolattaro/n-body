//
// Created by Jackson Campolattaro on 8/14/23.
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

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);

    // Load all .std files in the MUGS data directory
    std::filesystem::path directory{"../n-body-scenarios/benchmark/mugs/"};
    std::list<Simulation> scenarios; // Linked list avoids copying simulations during sorting
    for (const auto &path: std::filesystem::directory_iterator{directory}) {
        if (path.path().extension().string() != ".std") continue;
        spdlog::debug("Loading \"{}\"", path.path().string());
        scenarios.emplace_back();
        std::ifstream in{path};
        from_tipsy(in, scenarios.back());
        spdlog::debug(scenarios.back().size());
    }

    // Sort scenarios by size (for ascending tests)
    scenarios.sort([](const auto &s1, const auto &s2) {
        return s1.size() < s2.size();
    });

    // Create a grader based on the smallest scenario
    ConstitutionalGrader grader{std::filesystem::path{"../n-body-scenarios/benchmark/mugs/g7124.std"}};

    // Select an output file
    std::ofstream out{argc > 1 ? std::string{argv[1]} : "benchmarks/agora/all-solvers-constitutional.csv"};

    // Run the tests on each solver
    timeSolverOnScenarios<OctupoleBarnesHutSolver<Gravity>>("BH,Octupole", grader, scenarios, out);

    //    runFastTests<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR,Quadrupole",  out);
    //    runFastTests<OctupoleImplicitMVDRSolver<Gravity>>("MVDR,Octupole", out);
    //    //runFastTests<HexadecupoleImplicitMVDRSolver<Gravity>>("MVDR,Hexadecupole", out);
    //
    //    runFastTests<QuadrupoleImplicitFMMSolver<Gravity>>("FMM,Quadrupole",  out);
    //    runFastTests<OctupoleImplicitFMMSolver<Gravity>>("FMM,Octupole",  out);
    //    //runFastTests<HexadecupoleImplicitFMMSolver<Gravity>>("FMM,Hexadecupole",  out);
    //
    //    runFastTests<QuadrupoleLinearBVHSolver<Gravity>>("LBVH,Quadrupole",  out);
    //    runFastTests<OctupoleLinearBVHSolver<Gravity>>("LBVH,Octupole",  out);
    //    //runFastTests<HexadecupoleLinearBVHSolver<Gravity>>("LBVH,Hexadecupole",  out);
    //
    //    runFastTests<QuadrupoleBarnesHutSolver<Gravity>>("BH,Quadrupole",  out);
    //    runFastTests<OctupoleBarnesHutSolver<Gravity>>("BH,Octupole",  out);
    //    //runFastTests<HexadecupoleBarnesHutSolver<Gravity>>("BH,Hexadecupole",  out);

}
