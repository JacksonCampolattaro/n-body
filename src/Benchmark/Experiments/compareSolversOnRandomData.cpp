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

template<typename S>
void sweepNFixedTheta(const std::string &label, std::ostream &out, std::pair<int, int> range = {10'000, 1'000'000}) {

    // First, determine the best value of theta for this solver
    ConstitutionalGrader lowGrader{std::filesystem::path{"../n-body-scenarios/benchmark/LOW.bin"}};
    auto optimalTheta = lowGrader.optimalTheta<S>();
    spdlog::info("Using theta = {}", optimalTheta);

    // Next, time it for several simulations
    for (std::size_t n = range.first; n < range.second; n = n * 1.5) {

        Simulation scenario = Generator::createScenario(Generator::uniformRandomVolume, n);
        Gravity rule{};

        S solver{scenario, rule};
        solver.descentCriterion().theta() = optimalTheta;
        auto time = timedStep(solver);
        spdlog::info("{}, {} --> {}s", solver.name(), n, time.count());

        // If any scenario takes too long to run, stop early!
        if (time.count() > 60.0f) break;

        spdlog::debug("Running one step and counting interactions");
        Simulation trackingSimulation = scenario;
        SimpleTrackingRule<Gravity> trackingRule{solver.rule()};
        ReplaceRule<S, SimpleTrackingRule<Gravity>> interactionTrackingSolver{trackingSimulation, trackingRule};
        interactionTrackingSolver.descentCriterion().theta() = solver.descentCriterion().theta();
        interactionTrackingSolver.step();
        spdlog::debug("Interactions = {}", trackingRule.toString());
        float approximationRatio = (float) trackingRule.totalCount() / (float) std::pow(scenario.particleCount(), 2);
        spdlog::debug("Approximation Ratio = {}", approximationRatio);

        out << label << ","
            << scenario.particleCount() << ","
            << solver.descentCriterion().theta() << ","
            << time.count() << ","
            << trackingRule.particleParticleCount() << ","
            << trackingRule.particleNodeCount() << ","
            << trackingRule.nodeParticleCount() << ","
            << trackingRule.nodeNodeCount() << ","
            << approximationRatio << "\n" << std::flush;
    }
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    std::ofstream out{argc > 1 ? std::string{argv[1]} : "benchmarks/uniform/all-solvers.csv"};
    out << "Solver,Multipole Order,N,Theta,Time,"
           "Particle-Particle,Particle-Node,Node-Particle,Node-Node,"
           "Approximation Ratio\n";

    sweepNFixedTheta<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR,Quadrupole", out);
    sweepNFixedTheta<OctupoleImplicitMVDRSolver<Gravity>>("MVDR,Octupole", out);
    //sweepNFixedTheta<HexadecupoleImplicitMVDRSolver<Gravity>>("MVDR,Hexadecupole", out);

    sweepNFixedTheta<QuadrupoleImplicitFMMSolver<Gravity>>("FMM,Quadrupole", out);
    sweepNFixedTheta<OctupoleImplicitFMMSolver<Gravity>>("FMM,Octupole", out);
    //sweepNFixedTheta<HexadecupoleImplicitFMMSolver<Gravity>>("FMM,Hexadecupole", out);

    sweepNFixedTheta<QuadrupoleLinearBVHSolver<Gravity>>("LBVH,Quadrupole", out);
    sweepNFixedTheta<OctupoleLinearBVHSolver<Gravity>>("LBVH,Octupole", out);
    //sweepNFixedTheta<HexadecupoleLinearBVHSolver<Gravity>>("LBVH,Hexadecupole", out);

    sweepNFixedTheta<QuadrupoleBarnesHutSolver<Gravity>>("BH,Quadrupole", out);
    sweepNFixedTheta<OctupoleBarnesHutSolver<Gravity>>("BH,Octupole", out);
    //sweepNFixedTheta<HexadecupoleBarnesHutSolver<Gravity>>("BH,Hexadecupole", out);


}
