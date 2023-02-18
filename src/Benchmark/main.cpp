//
// Created by Jackson Campolattaro on 1/8/23.
//

#include "Comparison.h"
#include "ConstitutionalGrader.h"
#include "Generator.h"

#include <gtkmm.h>
#include <matplot/matplot.h>

#include <spdlog/spdlog.h>

#include <boost/progress.hpp>

#include <NBody/Physics/Rule.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/MVDRSolver.h>
#include <NBody/Simulation/Solvers/OctreeDualTraversalSolver.h>

using namespace NBody;

template<typename SolverType>
std::chrono::duration<float> timedStep(SolverType &solver) {
    auto startTime = std::chrono::steady_clock::now();
    solver.step();
    auto finishTime = std::chrono::steady_clock::now();
    return (finishTime - startTime);
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
void compare(std::size_t n, float theta) {

    json scenario = Generator::createScenario(&Generator::uniformRandomVolume, n, 50);

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
float appropriateTheta(json scenario, json referenceScenario, float allowableError,
                       const Comparison::Comparator &comparator, const Comparison::Metric &metric,
                       std::pair<float, float> range = {0.1, 3.0}) {

    // Binary search is partitioned around this value
    float middleValue = (range.first + range.second) / 2.0f;

    // After we've narrowed the range down enough, we can take the middle value
    if (range.second - range.first < 0.1f)
        return middleValue;

    Rule rule{};
    Simulation reference, candidate;
    from_json(referenceScenario, reference);
    from_json(scenario, candidate);

    CandidateSolver candidateSolver{candidate, rule};
    candidateSolver.theta() = middleValue;
    candidateSolver.step();

    auto error = comparator(metric, reference, candidate);

    if (error <= allowableError)
        return appropriateTheta<CandidateSolver>(scenario, referenceScenario, allowableError,
                                                 comparator, metric,
                                                 {range.first, middleValue});
    else
        return appropriateTheta<CandidateSolver>(scenario, referenceScenario, allowableError,
                                                 comparator, metric,
                                                 {middleValue, range.second});
}

template<typename ReferenceSolver, typename CandidateSolver>
float appropriateTheta(json scenario, float referenceTheta, std::pair<float, float> range = {0.1, 3.0}) {

    // Binary search is partitioned around this value
    float middleValue = (range.first + range.second) / 2.0f;

    // After we've narrowed the range down enough, we can take the middle value
    if (range.second - range.first < 0.1f)
        return middleValue;

    // Determine if accuracy is too high or too low
    Rule rule{};
    Simulation baseline, reference, candidate;
    from_json(scenario, baseline);
    from_json(scenario, reference);
    from_json(scenario, candidate);
    NaiveSolver baselineSolver{baseline, rule};
    ReferenceSolver referenceSolver{reference, rule};
    referenceSolver.theta() = referenceTheta;
    CandidateSolver candidateSolver{candidate, rule};
    candidateSolver.theta() = middleValue;
    baselineSolver.step();
    referenceSolver.step();
    candidateSolver.step();
    float referenceError = Comparison::average(&Comparison::L2Norm, baseline, reference);
    float candidateError = Comparison::average(&Comparison::L2Norm, baseline, candidate);
    spdlog::info("{} --> {} (vs {})", middleValue, candidateError, referenceError);
    if (referenceError <= candidateError)
        return appropriateTheta<ReferenceSolver, CandidateSolver>(scenario, referenceTheta,
                                                                  {range.first, middleValue});
    else
        return appropriateTheta<ReferenceSolver, CandidateSolver>(scenario, referenceTheta,
                                                                  {middleValue, range.second});


}

template<typename CandidateSolver>
void sweepTheta(std::size_t n, const std::vector<float> &thetaValues) {

    //json scenario = Generator::createScenario(&Generator::realisticGalaxy, n, 0);
    json scenario = Generator::createScenario(&Generator::uniformRandomVolume, n, 5);
    Rule rule{};

    spdlog::info("Performing naive integration to determine baseline");
    ConstitutionalGrader grader{scenario, rule};
    //Simulation baseline;
    //from_json(scenario, baseline);
    //NaiveSolver baselineSolver(baseline, rule);
    //timedStep(baselineSolver);

    std::map<std::string, std::vector<float>> results{
            {"theta", {}},
            {"time",  {}},
            {"error",    {}}
    };

    spdlog::info("Performing tests for different values of theta");
    boost::progress_display display(thetaValues.size());
    for (float theta: thetaValues) {

        Simulation simulation;
        from_json(scenario, simulation);

        CandidateSolver solver{simulation, rule};
        solver.theta() = theta;

        results["theta"].emplace_back(theta);
        results["time"].emplace_back(timedStep(solver).count());
        results["error"].emplace_back(grader.averageError(simulation));

        ++display;
    }

    // todo: there should be a way to get a solver's name statically
    Simulation simulation;
    CandidateSolver s{simulation, rule};

    matplot::title(fmt::format(
            "Error and Compute time of a {} solver for different values of θ ({} particles)",
            s.name(), scenario["particles"].size()
    ));
    matplot::xlabel("θ");
    matplot::plot(results["theta"], results["error"])->line_width(2.0f);
    matplot::ylabel("Error (Constitutional)");
    matplot::yticks({0.5});
    matplot::hold(true);
    matplot::plot(results["theta"], results["time"])->line_width(2.0f).use_y2(true);
    matplot::y2label("Iteration Time (S)");
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

        json scenario = Generator::createScenario(&Generator::uniformRandomVolume, n, 50);

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
    matplot::xlabel("# of random particles (n)");
    matplot::plot(results["n"], results["time"])->line_width(2.0f);
    matplot::ylabel("Iteration Time (S)");
    matplot::show();
}


void sweepN(const std::vector<std::size_t> &nValues, float theta, std::size_t i) {

    Rule rule{};

    std::map<std::string, std::vector<float>> results{
            {"n",    {}},
            {"time", {}},
    };

    for (std::size_t n: nValues) {

        json scenario = Generator::createScenario(&Generator::uniformRandomVolume, n, 50);

        Simulation barnesHutSimulation;
        from_json(scenario, barnesHutSimulation);
        Simulation linearBVHSimulation;
        from_json(scenario, linearBVHSimulation);
        Simulation mvdrSimulation;
        from_json(scenario, mvdrSimulation);
        Simulation dualTraversalSimulation;
        from_json(scenario, dualTraversalSimulation);

        BarnesHutSolver barnesHutSolver{barnesHutSimulation, rule};
        barnesHutSolver.theta() = theta;
        spdlog::info("{} theta = {}", barnesHutSolver.name(), barnesHutSolver.theta());
        LinearBVHSolver linearBVHSolver{linearBVHSimulation, rule};
        linearBVHSolver.theta() = appropriateTheta<BarnesHutSolver, LinearBVHSolver>(scenario, theta);
        spdlog::info("{} theta = {}", linearBVHSolver.name(), linearBVHSolver.theta());
        MVDRSolver mvdrSolver{mvdrSimulation, rule};
        mvdrSolver.theta() = appropriateTheta<BarnesHutSolver, MVDRSolver>(scenario, theta);
        spdlog::info("{} theta = {}", mvdrSolver.name(), mvdrSolver.theta());
        OctreeDualTraversalSolver dualTraversalSolver{mvdrSimulation, rule};
        dualTraversalSolver.theta() = appropriateTheta<BarnesHutSolver, OctreeDualTraversalSolver>(scenario, theta);
        spdlog::info("{} theta = {}", dualTraversalSolver.name(), dualTraversalSolver.theta());

        results["n"].emplace_back(n);
        results["barnes-hut-time"].emplace_back(timedRun(barnesHutSolver, i).count() / (float) i);
        results["linear-bvh-time"].emplace_back(timedRun(linearBVHSolver, i).count() / (float) i);
        results["mvdr-time"].emplace_back(timedRun(mvdrSolver, i).count() / (float) i);
        results["dual-traversal-time"].emplace_back(timedRun(dualTraversalSolver, i).count() / (float) i);
    }

    matplot::title(fmt::format(
            "Compute times of each solver for different simulation sizes (θ={})", theta
    ));
    matplot::xlabel("# of random particles (n)");
    matplot::ylabel("Iteration Time (S)");
    matplot::hold(true);
    matplot::plot(results["n"], results["barnes-hut-time"])->line_width(2.0f);
    matplot::plot(results["n"], results["linear-bvh-time"])->line_width(2.0f);
    matplot::plot(results["n"], results["mvdr-time"])->line_width(2.0f);
    matplot::plot(results["n"], results["dual-traversal-time"])->line_width(2.0f);
    matplot::legend({"Barnes-Hut", "Linear-BVH", "MVDR", "Dual-Traversal"});
    matplot::show();
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    Glib::init();

    std::vector<float> thetaValues{};
    for (int i = 1; i < 10; i++) thetaValues.emplace_back((float) i / 10.0f);
    sweepTheta<FMMSolver>(100'000, thetaValues);
    //sweepTheta<QuadrupoleBarnesHutSolver>(100'000, thetaValues);
    //sweepTheta<BarnesHutSolver>(100'000, {0.5});

    //std::vector<std::size_t> nValues{};
    //for (int i = 50'000; i < 1'000'000; i *= 1.5) nValues.emplace_back(i);
    //sweepN(nValues, 0.8, 5);
    //sweepN<MVDRSolver>(nValues, 0.8, 5);
    //sweepN(nValues, 1.0, 5);


    //json scenario = Generator::createScenario(&Generator::uniformRandomVolume, 100'000, 10);
}
