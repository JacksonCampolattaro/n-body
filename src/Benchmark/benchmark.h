//
// Created by Jackson Campolattaro on 5/31/23.
//

#ifndef N_BODY_BENCHMARK_H
#define N_BODY_BENCHMARK_H

#include <spdlog/spdlog.h>
#include <boost/progress.hpp>

#include <NBody/Physics/Rules/SimpleTrackingRule.h>

#include "Graders/Grader.h"

std::chrono::duration<float> timedInvoke(const std::function<void()>& function) {
    auto startTime = std::chrono::steady_clock::now();
    function();
    auto finishTime = std::chrono::steady_clock::now();
    return (finishTime - startTime);
}

template<typename CandidateSolver>
std::chrono::duration<float> timedStep(CandidateSolver &solver) {
    auto startTime = std::chrono::steady_clock::now();
    solver.step();
    auto finishTime = std::chrono::steady_clock::now();
    return (finishTime - startTime);
}

template<typename CandidateSolver>
std::chrono::duration<float> timedRun(CandidateSolver &solver, std::size_t iterations) {

    // Don't bother with a progress bar if it's only one step
    if (iterations == 1) return timedStep(solver);

    spdlog::debug("Running solver \"{}\" for {} iteration(s)", solver.name(), iterations);
    boost::progress_display display(iterations);
    auto startTime = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        solver.step();
        ++display;
    }
    auto finishTime = std::chrono::steady_clock::now();
    std::cout << std::endl;
    return (finishTime - startTime) / iterations;
}

template<class CandidateSolver>
void runTest(const std::string &label, const Grader &grader, std::ofstream &out, std::size_t iterations = 10) {

    spdlog::info("Running a battery of tests on solver with label \"{}\"", label);

    Simulation simulation;
    from_json(grader.scenario(), simulation);
    Gravity rule = grader.rule();

    spdlog::info("Finding an appropriate value of Theta for the solver");
    CandidateSolver solver{simulation, rule};
    solver.descentCriterion().theta() = grader.optimalTheta<CandidateSolver>();
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

template<class CandidateSolver>
void runShortTest(const std::string &label, const Grader &grader, std::ofstream &out, std::size_t iterations = 10) {

    spdlog::info("Running a battery of tests on solver with label \"{}\"", label);

    Simulation simulation;
    from_json(grader.scenario(), simulation);
    Gravity rule = grader.rule();

    spdlog::info("Finding an appropriate value of Theta for the solver");
    CandidateSolver solver{simulation, rule};
    solver.descentCriterion().theta() = grader.optimalTheta<CandidateSolver>();
    spdlog::info("Theta = {}", solver.descentCriterion().theta());

    spdlog::info("Timing the solver over {} iterations", iterations);
    for (int i = 0; i < iterations; ++i) {
        auto time = timedStep(solver);
        out << label << ","
            << simulation.particleCount() << ","
            << solver.descentCriterion().theta() << ","
            << i << ","
            << time.count() << "\n";
        spdlog::info("{}s", time.count());
    }

    spdlog::info("Done");
}

template<typename TreeType>
void testTreeConstruction(const std::string &label, Simulation &simulation, std::ofstream &out) {
    TreeType tree{simulation};

    auto time = timedInvoke([&]() { tree.build(); });

    out << label << ","
        << simulation.particleCount() << ","
        << tree.maxLeafSize() << ","
        << time.count() << "\n";
}

#endif //N_BODY_BENCHMARK_H
