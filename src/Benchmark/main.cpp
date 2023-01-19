//
// Created by Jackson Campolattaro on 1/8/23.
//

#include <random>
#include <gtkmm.h>

#include <spdlog/spdlog.h>

#include <boost/progress.hpp>

#include <NBody/Physics/Rule.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/DualTreeSolver.h>

#include "random.h"

using namespace NBody;

float L2Norm(const Simulation &a, const Simulation &b) {
    auto aValues = a.view<const Position, const Mass>();
    auto bValues = b.view<const Position, const Mass>();
    return std::transform_reduce(aValues.begin(), aValues.end(), bValues.begin(), 0.0f,
                                 std::plus{},
                                 [&](const auto &x, const auto &y) {
                                     return glm::distance2(
                                             (glm::vec3) a.get<Position>(x),
                                             (glm::vec3) b.get<Position>(y)
                                     );
                                 }
    ) / (float) std::distance(aValues.begin(), aValues.end());
}

float weightedL2Norm(const Simulation &a, const Simulation &b) {
    auto aValues = a.view<const Position, const Mass>();
    auto bValues = b.view<const Position, const Mass>();
    return std::transform_reduce(aValues.begin(), aValues.end(), bValues.begin(), 0.0f,
                                 std::plus{},
                                 [&](const auto &x, const auto &y) {
                                     return glm::distance2(
                                             (glm::vec3) a.get<Position>(x),
                                             (glm::vec3) b.get<Position>(y)
                                     ) * std::pow(a.get<Mass>(x).mass(), 2.0f);
                                 }
    ) / (float) std::distance(aValues.begin(), aValues.end());
}

float maxSquaredError(const Simulation &a, const Simulation &b) {
    auto aValues = a.view<const Position>();
    auto bValues = b.view<const Position>();
    return std::transform_reduce(aValues.begin(), aValues.end(), bValues.begin(), 0.0f,
                                 [&](float x, float y) {
                                     return std::max(x, y);
                                 },
                                 [&](const auto &x, const auto &y) {
                                     return glm::distance2(
                                             (glm::vec3) a.get<Position>(x),
                                             (glm::vec3) b.get<Position>(y)
                                     );
                                 }
    );
}

float fractionalError(const std::function<float(const Simulation &, const Simulation &)> &metric,
                      const Simulation &baseline, const Simulation &reference, const Simulation &candidate) {
    return metric(baseline, candidate) / metric(baseline, reference);
}

template<typename SolverType>
std::chrono::duration<float> timedRun(SolverType &solver, std::size_t iterations) {
    spdlog::info("Running solver \"{}\" for {} iteration(s)", solver.name(), iterations);
    boost::progress_display display(iterations);
    auto startTime = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        solver.step();
        ++display;
    }
    auto finishTime = std::chrono::steady_clock::now();
    std::cout << std::endl;
    return (finishTime - startTime);
}

template<typename ReferenceSolver, typename CandidateSolver>
void compare(std::size_t n, std::size_t i, float theta) {

    json scenario = randomSimulation(n);

    Simulation baseline;
    from_json(scenario, baseline);
    Simulation reference;
    from_json(scenario, reference);
    Simulation candidate;
    from_json(scenario, candidate);

    Rule rule{};

    NaiveSolver baselineSolver(baseline, rule);
    ReferenceSolver referenceSolver(reference, rule);
    referenceSolver.theta() = theta;
    CandidateSolver candidateSolver(candidate, rule);
    candidateSolver.theta() = theta;

    auto baselineTime = timedRun(baselineSolver, i);
    auto referenceTime = timedRun(referenceSolver, i);
    auto candidateTime = timedRun(candidateSolver, i);

    float L2 = fractionalError(&L2Norm, baseline, reference, candidate);
    float weightedL2 = fractionalError(&weightedL2Norm, baseline, reference, candidate);
    float maxSquared = fractionalError(&maxSquaredError, baseline, reference, candidate);
    spdlog::info("{} Error (as a % of {} error):\n"
                 "\tL2\t\t\t= {}%\n"
                 "\tWeighted L2\t= {}%\n"
                 "\tMax Squared\t= {}%\n",
                 candidateSolver.name(), referenceSolver.name(),
                 L2 * 100.0f, weightedL2 * 100.0f, maxSquared * 100.0f);

    float relativeRuntime = candidateTime.count() / referenceTime.count();
    float speedup = candidateTime.count() / baselineTime.count();
    spdlog::info("{} Performance:\n"
                 "\t{} % of {} runtime\n"
                 "\t{} % of {} runtime\n",
                 candidateSolver.name(),
                 relativeRuntime * 100.0f, referenceSolver.name(),
                 speedup * 100.0f, baselineSolver.name()
    );
}

template<typename CandidateSolver>
void sweepTheta(std::size_t n, const std::vector<float> &thetaValues) {

    json scenario = randomSimulation(n);

    Rule rule{};

    Simulation baseline;
    from_json(scenario, baseline);
    NaiveSolver baselineSolver(baseline, rule);
    timedRun(baselineSolver, 100);

    std::map<std::string, std::vector<float>> results{
            {"theta", {}},
            {"time", {}},
            {"l2", {}}
    };


    for (float theta: thetaValues) {

        Simulation simulation;
        from_json(scenario, simulation);

        CandidateSolver solver{simulation, rule};
        solver.theta() = theta;

        auto time = timedRun(solver, 100);
        auto l2Norm = L2Norm(baseline, simulation);

        results["theta"].emplace_back(theta);
        results["time"].emplace_back(time.count());
        results["l2"].emplace_back(l2Norm);
    }

    for (int i = 0; i < thetaValues.size(); ++i) {
        spdlog::info("{} --> {}", results["theta"][i], results["l2"][i]);
    }

}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    Glib::init();

    //compare<BarnesHutSolver, LinearBVHSolver>(5000, 100, 0.5);

    std::vector<float> thetaValues = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5};
    sweepTheta<BarnesHutSolver>(5000, thetaValues);
}
