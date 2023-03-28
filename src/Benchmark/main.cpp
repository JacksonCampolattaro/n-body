//
// Created by Jackson Campolattaro on 1/8/23.
//

#include "Comparison.h"
#include "ConstitutionalGrader.h"
#include "MeanGrader.h"
#include "RMSGrader.h"
#include "Generator.h"
#include "bestTheta.h"

#include <gtkmm.h>
#include <matplot/matplot.h>

#include <spdlog/spdlog.h>

#include <boost/progress.hpp>

#include <NBody/Physics/Rule.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/ReverseBarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/MVDRSolver.h>
#include <NBody/Simulation/Solvers/OctreeDualTraversalSolver.h>
#include <NBody/Simulation/Solvers/FMMSolver.h>

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

    // Don't bother with a progress bar if it's only one step
    if (iterations == 1) return timedStep(solver);

    spdlog::info("Running solver \"{}\" for {} iteration(s)", solver.name(), iterations);
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

template<typename CandidateSolver>
void sweepTheta(json scenario, const std::vector<float> &thetaValues) {

    Rule rule{};

    ConstitutionalGrader grader{scenario, rule};

    std::map<std::string, std::vector<float>> results{
            {"theta", {}},
            {"time",  {}},
            {"error", {}}
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
        results["error"].emplace_back(grader.error(simulation));

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
        //barnesHutSolver.theta() = theta;
        spdlog::info("{} theta = {}", barnesHutSolver.name(), barnesHutSolver.theta());
        LinearBVHSolver linearBVHSolver{linearBVHSimulation, rule};
        //linearBVHSolver.theta() = appropriateTheta<BarnesHutSolver, LinearBVHSolver>(scenario, theta);
        spdlog::info("{} theta = {}", linearBVHSolver.name(), linearBVHSolver.theta());
        MVDRSolver mvdrSolver{mvdrSimulation, rule};
        //mvdrSolver.theta() = appropriateTheta<BarnesHutSolver, MVDRSolver>(scenario, theta);
        spdlog::info("{} theta = {}", mvdrSolver.name(), mvdrSolver.theta());
        OctreeDualTraversalSolver dualTraversalSolver{mvdrSimulation, rule};
        //dualTraversalSolver.theta() = appropriateTheta<BarnesHutSolver, OctreeDualTraversalSolver>(scenario, theta);
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

template<typename CandidateSolver>
float accuracy(json scenario, const Grader &grader, float theta = 0.5) {

    // Create a solver
    Rule rule{};
    Simulation simulation;
    from_json(scenario, simulation);
    CandidateSolver solver{simulation, rule};
    solver.theta() = theta;

    // Evaluate forces
    solver.step();

    // Check its accuracy
    return grader.error(simulation);
}

template<typename CandidateSolver>
std::chrono::duration<float> performance(json scenario, int i, float theta = 0.5) {

    // Create a solver
    Rule rule{};
    Simulation simulation;
    from_json(scenario, simulation);
    CandidateSolver solver{simulation, rule};
    solver.theta() = theta;

    return timedRun(solver, i);
}

template<typename CandidateSolver>
std::chrono::duration<float> realPerformance(json scenario, const Grader &grader, int iterations = 1) {

    // Create a solver
    Rule rule{};
    Simulation simulation;
    from_json(scenario, simulation);
    CandidateSolver solver{simulation, rule};

    // Select a value of theta that can produce the necessary accuracy
    solver.theta() = searchTheta<CandidateSolver>(scenario, grader);

    // Time the solver
    auto time = timedRun(solver, iterations);
    spdlog::info("{} (θ={}) --> {} s / iteration", solver.name(), solver.theta(), time.count());
    return time;
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    Glib::init();
    // Limit to 1 thread when debugging
    //tbb::global_control c{tbb::global_control::max_allowed_parallelism, 1};

    //json scenario = Generator::realisticGalaxy();
    json scenario = Generator::createScenario(Generator::uniformRandomVolume, 10'000, 0);

    //MeanGrader grader{scenario};
    ConstitutionalGrader grader{scenario};

    std::vector<float> thetaValues{};
    for (int i = 1; i < 10; i++) thetaValues.emplace_back((float) i / 10.0f);
    //sweepTheta<FMMSolver>(scenario, thetaValues);
    //sweepTheta<MVDRSolver>(scenario, thetaValues);
    //sweepTheta<QuadrupoleMVDRSolver>(scenario, thetaValues);
    //sweepTheta<BarnesHutSolver>(scenario, thetaValues);
    //sweepTheta<QuadrupoleBarnesHutSolver>(scenario, thetaValues);
    //sweepTheta<OctupoleBarnesHutSolver>(scenario, thetaValues);
    //sweepTheta<LinearBVHSolver>(scenario, thetaValues);
    //sweepTheta<ReverseBarnesHutSolver>(scenario, thetaValues);


    //realPerformance<BarnesHutSolver>(scenario, grader);
    //realPerformance<QuadrupoleBarnesHutSolver>(scenario, grader);
    //realPerformance<OctupoleBarnesHutSolver>(scenario, grader);
    //realPerformance<ReverseBarnesHutSolver>(scenario, grader);
    //realPerformance<QuadrupoleReverseBarnesHutSolver>(scenario, grader);
    //realPerformance<LinearBVHSolver>(scenario, grader);
    //realPerformance<QuadrupoleLinearBVHSolver>(scenario, grader);
    //realPerformance<OctupoleLinearBVHSolver>(scenario, grader);
    //realPerformance<FMMSolver>(scenario, grader);
    //realPerformance<QuadrupoleFMMSolver>(scenario, grader);
    //realPerformance<OctupoleFMMSolver>(scenario, grader);
    //realPerformance<MVDRSolver>(scenario, grader);
    realPerformance<QuadrupoleMVDRSolver>(scenario, grader);
    realPerformance<OctupoleMVDRSolver>(scenario, grader);

    //spdlog::info(accuracy<ReverseBarnesHutSolver>(scenario, grader, 0.5));
    //spdlog::info(performance<ReverseBarnesHutSolver>(scenario, 1, 0.5).count());

    //std::vector<std::size_t> nValues{};
    //for (int i = 50'000; i < 1'000'000; i *= 1.5) nValues.emplace_back(i);
    //sweepN(nValues, 0.8, 5);
    //sweepN<MVDRSolver>(nValues, 0.8, 5);
    //sweepN(nValues, 1.0, 5);


    //json scenario = Generator::createScenario(&Generator::uniformRandomVolume, 100'000, 10);
}
