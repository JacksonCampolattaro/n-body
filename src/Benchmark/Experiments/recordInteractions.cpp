//
// Created by Jackson Campolattaro on 6/21/23.
//
#include <gtkmm.h>
#include <spdlog/spdlog.h>

#include <NBody/Physics/Rules/Gravity.h>
#include <NBody/Physics/Rules/AdvancedTrackingRule.h>

#include <NBody/Simulation/Simulation.h>

#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/ImplicitReverseBarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/ImplicitFMMSolver.h>
#include <NBody/Simulation/Solvers/ImplicitMVDRSolver.h>
#include <NBody/Simulation/Solvers/FMMSolver.h>
#include <NBody/Simulation/Solvers/MVDRSolver.h>

#include "../Generator.h"
#include "../Graders/ConstitutionalGrader.h"

template<typename CandidateSolver>
void recordInteractions(NBody::Grader &grader, std::filesystem::path outputPath) {

    float theta = grader.optimalTheta<CandidateSolver>();
    AdvancedTrackingRule<Gravity> trackingRule{
            outputPath.parent_path().string() + "/" + outputPath.stem().string() + fmt::format("-theta={}.csv", theta),
            0.01,
            grader.rule()
    };
    Simulation simulation = grader.scenario();
    ReplaceRule<CandidateSolver, AdvancedTrackingRule<Gravity>> interactionTrackingSolver{simulation, trackingRule};
    interactionTrackingSolver.descentCriterion().theta() = theta;
    //interactionTrackingSolver.tree().maxLeafSize() = 1;
    spdlog::info(
            "Counting interactions for solver {} with theta = {}",
            interactionTrackingSolver.name(),
            theta
    );
    interactionTrackingSolver.step();
    spdlog::info("Done");
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    //    Simulation simulation = Generator::createScenario(Generator::perlinNoiseRandomVolume, 10'000);
    //    ConstitutionalGrader grader(simulation);
    ConstitutionalGrader grader{std::filesystem::path{"../n-body-scenarios/benchmark/LOW.bin"}};

    //    recordInteractions<QuadrupoleBarnesHutSolver<Gravity>>(
    //            grader,
    //            argc > 1 ? std::string{argv[1]} : "benchmarks/bh-2-interactions.csv"
    //    );

    //    recordInteractions<QuadrupoleLinearBVHSolver<Gravity>>(
    //            grader,
    //            argc > 1 ? std::string{argv[1]} : "benchmarks/lbvh-bh-2-interactions.csv"
    //    );

    //    recordInteractions<QuadrupoleMVDRSolver<Gravity>>(
    //            grader,
    //            argc > 1 ? std::string{argv[1]} : "benchmarks/mvdr-2-interactions.csv"
    //    );

    recordInteractions<QuadrupoleFMMSolver<Gravity>>(
            grader,
            argc > 1 ? std::string{argv[1]} : "benchmarks/fmm-2-interactions.csv"
    );
}
