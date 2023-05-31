//
// Created by Jackson Campolattaro on 5/31/23.
//

#include <gtkmm.h>
#include <spdlog/spdlog.h>

#include <NBody/Physics/Rules/Gravity.h>
#include <NBody/Physics/Rules/SimpleTrackingRule.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>

#include "../Generator.h"
#include "../bestTheta.h"
#include "../benchmark.h"

template<class CandidateSolver>
void runTest(const std::string &label, const Grader &grader, std::ofstream &out, std::size_t iterations = 10) {

    spdlog::info("Running a battery of tests on solver with label \"{}\"", label);

    Simulation simulation;
    from_json(grader.scenario(), simulation);
    Gravity rule = grader.rule();

    spdlog::info("Finding an appropriate value of Theta for the solver");
    CandidateSolver solver{simulation, rule};
    solver.descentCriterion().theta() = searchTheta<CandidateSolver>(grader);
    spdlog::info("Theta = {}", solver.descentCriterion().theta());

    spdlog::info("Timing the solver over {} iterations", iterations);
    auto time = timedRun(solver, iterations);
    spdlog::info("Average of {}s / iteration", time.count());

    spdlog::info("Running one step and counting interactions");
    Simulation trackingSimulation;
    from_json(grader.scenario(), trackingSimulation);
    SimpleTrackingRule<Gravity> trackingRule{solver.rule()};
    ReplaceRule<CandidateSolver, SimpleTrackingRule<Gravity>> interactionTrackingSolver{trackingSimulation,
                                                                                        trackingRule};
    interactionTrackingSolver.descentCriterion().theta() = solver.descentCriterion().theta();
    interactionTrackingSolver.step();
    spdlog::info("Interactions = {}", trackingRule.toString());
    float approximationRatio = (float) trackingRule.totalCount() / (float) std::pow(simulation.particleCount(), 2);
    spdlog::info("Approximation Ratio = {}", approximationRatio);

    spdlog::info("Recording results to CSV");
    out << label << ","
        << simulation.particleCount() << ","
        << solver.descentCriterion().theta() << ","
        << time.count() << ","
        << trackingRule.particleParticleCount() << ","
        << trackingRule.particleNodeCount() << ","
        << trackingRule.nodeParticleCount() << ","
        << trackingRule.nodeNodeCount() << ","
        << approximationRatio << "\n";

    spdlog::info("Done");
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    Glib::init();

    std::ofstream out{"benchmarks/linear-bvh-descent-criterion.csv"};
    out<< "Descent Criterion,N,Theta,Time,"
          "Particle-Particle,Particle-Node,Node-Particle,Node-Node,"
          "Approximation Ratio\n";

    json scenario = Generator::realisticGalaxy();
    Gravity rule{};

    ConstitutionalGrader grader{scenario, rule};

    runTest<
            ActiveTreeSolver<
                    LinearBVH<QuadrupoleMassSummary>,
                    Descent::DiagonalOverDistance,
                    Gravity
            >
    >(R"($\frac{\textnormal{Diagonal}}{\textnormal{Distance}}$)", grader, out);

    runTest<
            ActiveTreeSolver<
                    LinearBVH<QuadrupoleMassSummary>,
                    Descent::MaxSideLengthOverDistance,
                    Gravity
            >
    >(R"($\frac{\textnormal{Side Length}}{\textnormal{Distance}}$)", grader, out);

    runTest<
            ActiveTreeSolver<
                    LinearBVH<QuadrupoleMassSummary>,
                    Descent::DiagonalOverSeparation,
                    Gravity
            >
    >(R"($\frac{\textnormal{Diagonal}}{\textnormal{Separation}}$)", grader, out);

    runTest<
            ActiveTreeSolver<
                    LinearBVH<QuadrupoleMassSummary>,
                    Descent::DiagonalOverCenterDistance,
                    Gravity
            >
    >(R"($\frac{\textnormal{Diagonal}}{\textnormal{Center Distance}}$)", grader, out);

}
