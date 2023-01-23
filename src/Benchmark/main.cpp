//
// Created by Jackson Campolattaro on 1/8/23.
//

#include "Comparison.h"
#include "random.h"

#include <gtkmm.h>
#include <matplot/matplot.h>

#include <spdlog/spdlog.h>

#include <boost/progress.hpp>

#include <NBody/Physics/Rule.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/DualTreeSolver.h>

using namespace NBody;

//float fractionalError(const std::function<float(const Simulation &, const Simulation &)> &metric,
//                      const Simulation &baseline, const Simulation &reference, const Simulation &candidate) {
//    return metric(baseline, candidate) / metric(baseline, reference);
//}

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
void compare(std::size_t n, float theta) {

    json scenario = randomVolumeSimulation(n);

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

    auto baselineTime = timedRun(baselineSolver, 1);
    auto referenceTime = timedRun(referenceSolver, 1);
    auto candidateTime = timedRun(candidateSolver, 1);

    float averageL2 = Comparison::fractional(&Comparison::average, &Comparison::L2Norm, baseline, reference, candidate);
    float maximumL2 = Comparison::fractional(&Comparison::maximum, &Comparison::L2Norm, baseline, reference, candidate);
    spdlog::info("{} Comparison (as a % of {} error):\n"
                 "\taverage L2\t\t\t= {}%\n"
                 "\tmaximum L2\t= {}%\n",
                 candidateSolver.name(), referenceSolver.name(),
                 averageL2 * 100.0f, maximumL2 * 100.0f);

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

    json scenario = randomVolumeSimulation(n);

    Rule rule{};

    Simulation baseline;
    from_json(scenario, baseline);
    NaiveSolver baselineSolver(baseline, rule);
    timedRun(baselineSolver, 1);

    std::map<std::string, std::vector<float>> results{
            {"theta", {}},
            {"time",  {}},
            {"l2",    {}}
    };

    for (float theta: thetaValues) {

        Simulation simulation;
        from_json(scenario, simulation);

        CandidateSolver solver{simulation, rule};
        solver.theta() = theta;

        results["theta"].emplace_back(theta);
        results["time"].emplace_back(timedRun(solver, 1).count());
        results["l2"].emplace_back(Comparison::average(&Comparison::L2Norm, baseline, simulation));
    }

    // todo: there should be a way to get a solver's name statically
    CandidateSolver s{baseline, rule};

    matplot::title(fmt::format(
            "Error and Compute time of a {} solver for different values of θ ({} random particles)",
            s.name(), n
    ));
    matplot::xlabel("θ");
    matplot::plot(results["theta"], results["l2"], "-o");
    matplot::ylabel("Accuracy (average L2 error vs. Naive forces)");
    matplot::hold(true);
    matplot::plot(results["theta"], results["time"], "-o")->use_y2(true);
    matplot::y2label("Performance (seconds-per-iteration)");
    matplot::show();

}

template<typename CandidateSolver>
void sweepN(const std::vector<std::size_t> &nValues, float theta, std::size_t i) {

    Rule rule{};

    std::map<std::string, std::vector<float>> results{
            {"n",    {}},
            {"time", {}},
    };

    for (std::size_t n: nValues) {

        json scenario = randomVolumeSimulation(n);

        Simulation simulation;
        from_json(scenario, simulation);

        CandidateSolver solver{simulation, rule};
        solver.theta() = theta;

        results["n"].emplace_back(n);
        results["time"].emplace_back(timedRun(solver, i).count() / (float) i);
    }

    // todo: there should be a way to get a solver's name statically
    Simulation simulation;
    CandidateSolver s{simulation, rule};

    matplot::title(fmt::format(
            "Compute time of a {} solver for different simulation sizes (θ={})",
            s.name(), theta
    ));
    matplot::xlabel("Number of random particles (n)");
    matplot::plot(results["n"], results["time"], "-o");
    matplot::ylabel("Performance (seconds-per-iteration)");
    matplot::show();

}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    Glib::init();

    //randomGalaxySimulation(10'000);

    //compare<BarnesHutSolver, LinearBVHSolver>(10000, 100, 0.5);

    //std::vector<float> thetaValues = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5};
    //sweepTheta<LinearBVHSolver>(50'000, thetaValues);

    std::vector<std::size_t> nValues{};
    for (int i = 100; i < 1000000; i *= 1.25) nValues.emplace_back(i);
    sweepN<BarnesHutSolver>(nValues, 0.7, 5);
}
