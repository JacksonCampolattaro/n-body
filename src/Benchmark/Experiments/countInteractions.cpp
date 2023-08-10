//
// Created by Jackson Campolattaro on 8/10/23.
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
void countInteractionsOnAGORADatasets(const std::string &label, std::ostream &out) {

    // First, determine the best value of theta for this solver
    ConstitutionalGrader lowGrader{std::filesystem::path{"../n-body-scenarios/benchmark/LOW.bin"}};
    auto optimalTheta = lowGrader.optimalTheta<S>();

    // Next, time it for several simulations
    for (const std::string &dataset: {"LOW"/*, "MED", "HI"*/}) {

        Physics::Gravity rule{};
        Simulation scenario;
        std::ifstream scenarioFile{fmt::format("../n-body-scenarios/benchmark/{}.bin", dataset)};
        assert(scenarioFile.is_open());
        from_tipsy(scenarioFile, scenario);
        spdlog::debug("Loaded {} particles", scenario.particleCount());

        spdlog::debug("Running one step and counting interactions");
        SimpleTrackingRule<Gravity> trackingRule{rule};
        ReplaceRule<S, SimpleTrackingRule<Gravity>> interactionTrackingSolver{scenario, trackingRule};
        interactionTrackingSolver.descentCriterion().theta() = optimalTheta;
        interactionTrackingSolver.step();
        spdlog::info("{} (theta={}) --> {}", interactionTrackingSolver.name(), optimalTheta, trackingRule.toString());

        float approximationRatio = (float) trackingRule.totalCount() / (float) std::pow(scenario.particleCount(), 2);
        spdlog::debug("Approximation Ratio = {}", approximationRatio);

        out << label << ","
            << scenario.particleCount() << ","
            << interactionTrackingSolver.descentCriterion().theta() << ","
            << trackingRule.particleParticleCount() << ","
            << trackingRule.particleNodeCount() << ","
            << trackingRule.nodeParticleCount() << ","
            << trackingRule.nodeNodeCount() << ","
            << approximationRatio << std::endl;
    }
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    std::ofstream out{argc > 1 ? std::string{argv[1]} : "benchmarks/interactions/all-solvers-count.csv"};
    out << "Solver,Multipole Order,N,Theta,"
           "Particle-Particle,Particle-Node,Node-Particle,Node-Node,"
           "Approximation Ratio\n";

    countInteractionsOnAGORADatasets<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR,Quadrupole", out);
    countInteractionsOnAGORADatasets<QuadrupoleImplicitFMMSolver<Gravity>>("FMM,Quadrupole", out);
    countInteractionsOnAGORADatasets<QuadrupoleLinearBVHSolver<Gravity>>("LBVH,Quadrupole", out);
    countInteractionsOnAGORADatasets<QuadrupoleBarnesHutSolver<Gravity>>("BH,Quadrupole", out);

}
