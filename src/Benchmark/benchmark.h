//
// Created by Jackson Campolattaro on 5/31/23.
//

#ifndef N_BODY_BENCHMARK_H
#define N_BODY_BENCHMARK_H

#include <spdlog/spdlog.h>

#include <NBody/Physics/Rules/SimpleTrackingRule.h>

#include "Graders/Grader.h"

std::chrono::duration<float> timedInvoke(const std::function<void()> &function) {
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

    spdlog::debug("Running solver \"{}\" for {} iteration(s)", solver.name(), iterations);
    auto startTime = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        solver.step();
    }
    auto finishTime = std::chrono::steady_clock::now();
    return (finishTime - startTime) / iterations;
}

template<class CandidateSolver>
void runTest(const std::string &label, const Grader &grader, std::ofstream &out, std::size_t iterations = 5) {

    spdlog::debug("Running a battery of tests on solver \"{}\"", label);

    Simulation simulation = grader.scenario();
    Gravity rule = grader.rule();

    spdlog::debug("Finding an appropriate value of Theta for the solver");
    CandidateSolver solver{simulation, rule};
    solver.descentCriterion().theta() = grader.optimalTheta<CandidateSolver>();
    spdlog::debug("Theta = {}", solver.descentCriterion().theta());

    if (solver.descentCriterion().theta() == 0.1) return;

    spdlog::debug("Running one step and counting interactions");
    Simulation trackingSimulation = grader.scenario();
    SimpleTrackingRule<Gravity> trackingRule{solver.rule()};
    ReplaceRule<CandidateSolver, SimpleTrackingRule<Gravity>> interactionTrackingSolver{trackingSimulation,
                                                                                        trackingRule};
    interactionTrackingSolver.descentCriterion().theta() = solver.descentCriterion().theta();
    interactionTrackingSolver.step();
    spdlog::debug("Interactions = {}", trackingRule.toString());
    float approximationRatio = (float) trackingRule.totalCount() / (float) std::pow(simulation.particleCount(), 2);
    spdlog::debug("Approximation Ratio = {}", approximationRatio);

    spdlog::debug("Timing the solver for {} iterations", iterations);
    std::chrono::duration<float> time;
    for (int i = 0; i < iterations; ++i) {

        time = timedInvoke([&]() { solver.step(); });
        spdlog::debug("{}s", time.count());

        out << label << ","
            << simulation.particleCount() << ","
            << solver.descentCriterion().theta() << ","
            << time.count() << ","
            << trackingRule.particleParticleCount() << ","
            << trackingRule.particleNodeCount() << ","
            << trackingRule.nodeParticleCount() << ","
            << trackingRule.nodeNodeCount() << ","
            << approximationRatio << "\n" << std::flush;
    }
    spdlog::info("{} (theta={}) --> {}s", solver.name(), solver.descentCriterion().theta(), time.count());


}

template<class CandidateSolver>
void runShortTest(const std::string &label, const Grader &grader, std::ofstream &out, std::size_t iterations = 10) {

    spdlog::info("Running a battery of tests on solver \"{}\"", label);

    Simulation simulation = grader.scenario();
    Gravity rule = grader.rule();

    spdlog::debug("Finding an appropriate value of Theta for the solver");
    CandidateSolver solver{simulation, rule};
    solver.descentCriterion().theta() = grader.optimalTheta<CandidateSolver>();
    spdlog::info("Theta = {}", solver.descentCriterion().theta());

    spdlog::debug("Timing the solver over {} iterations", iterations);
    for (int i = 0; i < iterations; ++i) {
        auto time = timedStep(solver);
        out << label << ","
            << simulation.particleCount() << ","
            << solver.descentCriterion().theta() << ","
            << i << ","
            << time.count() << "\n" << std::flush;
        spdlog::info("({}/{}) {}s", i, iterations, time.count());
    }

}

template<class CandidateSolver>
void runFastTest(const std::string &label, const Grader &grader, std::ofstream &out, std::size_t iterations = 5) {

    spdlog::info("\nRunning a battery of tests on solver \"{}\"", label);

    Simulation simulation = grader.scenario();
    Gravity rule = grader.rule();

    CandidateSolver solver{simulation, rule};
    spdlog::info("Theta = {}", solver.descentCriterion().theta());

    spdlog::debug("Timing the solver over {} iterations", iterations);
    for (int i = 0; i < iterations; ++i) {
        auto time = timedStep(solver);
        out << label << ","
            << simulation.particleCount() << ","
            << solver.descentCriterion().theta() << ","
            << i << ","
            << time.count() << "\n" << std::flush;
        spdlog::info("({}/{}) {}s", i, iterations, time.count());
    }

}

template<class CandidateSolver>
void timeSolverOnScenarios(const std::string &label,
                  const Grader &grader, const std::list<Simulation> &scenarios,
                  std::ofstream &out) {

    // If the file is empty, add a header
    if (out.tellp() == 0)
        out << "Solver,Multipole Order,N,Theta,Time\n";

    // Select theta for this solver
    float theta = grader.optimalTheta<CandidateSolver>();
    spdlog::info("Using theta={}", theta);

    // Time the solver on each of the scenarios
    for (const auto &s: scenarios) {
        Simulation simulation{s};
        Physics::Gravity rule{};

        CandidateSolver solver{simulation, rule};
        solver.descentCriterion().theta() = theta;
        auto time = timedStep(solver);
        spdlog::info("{}, n={} --> {}s", solver.name(), simulation.particleCount(), time.count());
        out << label << ","
            << simulation.particleCount() << ","
            << theta << ","
            << time.count() << std::endl;

        // If any scenario takes too long to run, stop early!
        if (time.count() > 300.0f) break;
    }
}


template<typename TreeType>
void testTreeConstruction(const std::string &label, Simulation &simulation, std::ofstream &out) {
    TreeType tree{simulation};

    auto time = timedInvoke([&]() { tree.build(); });

    out << label << ","
        << simulation.particleCount() << ","
        << tree.maxLeafSize() << ","
        << tree.root().numberOfDescendants() << ","
        << time.count() << "\n" << std::flush;
}

#endif //N_BODY_BENCHMARK_H
