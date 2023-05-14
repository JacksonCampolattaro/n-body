//
// Created by Jackson Campolattaro on 1/8/23.
//

#include "Comparison.h"
#include "ConstitutionalGrader.h"
#include "MeanGrader.h"
#include "RMSGrader.h"
#include "Generator.h"
#include "bestTheta.h"
#include "field.h"
#include "gravitationalViewingSheet.h"

#include <gtkmm.h>
#include <matplot/matplot.h>

#include <spdlog/spdlog.h>

#include <boost/progress.hpp>

#include <NBody/Physics/Rule.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/ReverseBarnesHutSolver.h>
#include <NBody/Simulation/Solvers/ImplicitReverseBarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/MVDRSolver.h>
#include <NBody/Simulation/Solvers/ImplicitMVDRSolver.h>
#include <NBody/Simulation/Solvers/OctreeDualTraversalSolver.h>
#include <NBody/Simulation/Solvers/FMMSolver.h>
#include <NBody/Simulation/Solvers/ImplicitFMMSolver.h>

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

template<typename... Solvers>
void sweepN(const std::vector<std::size_t> &nValues, std::size_t iterations) {

    // Open a CSV file and add the header
    std::ofstream file{"benchmarks/sweep-n.csv"};
    file << "n,solver-id,Solver,θ,Time (s)" << std::endl;

    Rule rule{};

    for (std::size_t n: nValues) {

        json scenario = Generator::createScenario(&Generator::uniformRandomVolume, n);
        ConstitutionalGrader grader(scenario, rule);

        // For each solver being tested
        ([&] {

            // Set up the simulation
            Simulation simulation;
            from_json(scenario, simulation);
            Solvers solver(simulation, rule);

            // Find an appropriate value of theta
            solver.theta() = searchTheta<Solvers>(scenario, grader, {0.1, 0.9});

            // Time the simulation
            auto time = timedRun(solver, iterations);
            spdlog::info("{} (θ={}) --> {} s / iteration", solver.name(), solver.theta(), time.count());

            // Write the results to CSV
            file << fmt::format(
                    "{},{},{},{},{}\n",
                    n, solver.id(), solver.name(), solver.theta(), time.count()
            );

        }(), ...);
    }
}

template<typename... Solvers>
void sweepTheta(json scenario, const std::vector<float> &thetaValues) {

    // Open a CSV file and add the header
    std::ofstream file{"benchmarks/sweep-theta.csv"};
    file << "n,solver-id,Solver,θ,Time (s),% Error (Constitutional)" << std::endl;

    Rule rule{};

    for (float theta: thetaValues) {

        ConstitutionalGrader grader(scenario, rule);

        // For each solver being tested
        ([&] {

            // Set up the simulation
            Simulation simulation;
            from_json(scenario, simulation);
            Solvers solver(simulation, rule);
            solver.theta() = theta;

            // Time the simulation
            auto time = timedStep(solver);

            // Measure error
            auto error = grader.error(simulation);

            spdlog::info(
                    "{} (θ={}) --> {} s / iteration, {} % error",
                    solver.name(), solver.theta(), time.count(), error
            );

            // Write the results to CSV
            file << fmt::format(
                    "{},{},{},{},{},{}\n",
                    simulation.size(), solver.id(), solver.name(), solver.theta(), time.count(), error
            );

        }(), ...);
    }
}

template<typename CandidateSolver>
float accuracy(json scenario, const Grader &grader, float theta = 0.5) {

    // Create a solver
    Rule rule{grader.rule()};
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
    Rule rule = grader.rule();
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
    spdlog::set_level(spdlog::level::info);
    Glib::init();
    // Limit to 1 thread when debugging
    //tbb::global_control c{tbb::global_control::max_allowed_parallelism, 1};

    json scenario = Generator::realisticGalaxy();
    //json scenario = Generator::trio();
    //json scenario = Generator::createScenario(Generator::uniformRandomVolume, 10'000);

    //MeanGrader grader{scenario};
    //RMSGrader grader{scenario};
    ConstitutionalGrader grader{scenario, Rule{3.0f}};

    //plotExactField(scenario);
    //plotMomentApproximations(scenario);
    //plotFieldApproximations(scenario);


    //realPerformance<BarnesHutSolver>(scenario, grader);
    //realPerformance<QuadrupoleBarnesHutSolver>(scenario, grader);
    //realPerformance<OctupoleBarnesHutSolver>(scenario, grader);
    //realPerformance<ReverseBarnesHutSolver>(scenario, grader);
    //realPerformance<QuadrupoleReverseBarnesHutSolver>(scenario, grader);
    //realPerformance<QuadrupoleImplicitReverseBarnesHutSolver>(scenario, grader);
    //realPerformance<OctupoleReverseBarnesHutSolver>(scenario, grader);
    //realPerformance<HexadecapoleReverseBarnesHutSolver>(scenario, grader);
    //realPerformance<LinearBVHSolver>(scenario, grader);
    //realPerformance<QuadrupoleLinearBVHSolver>(scenario, grader);
    //realPerformance<OctupoleLinearBVHSolver>(scenario, grader);
    //realPerformance<FMMSolver>(scenario, grader);
    //realPerformance<QuadrupoleFMMSolver>(scenario, grader);
    //realPerformance<OctupoleFMMSolver>(scenario, grader);
    //realPerformance<ImplicitFMMSolver>(scenario, grader);
    //realPerformance<QuadrupoleImplicitFMMSolver>(scenario, grader);
    //realPerformance<OctupoleImplicitFMMSolver>(scenario, grader);
    //realPerformance<MVDRSolver>(scenario, grader);
    //realPerformance<QuadrupoleMVDRSolver>(scenario, grader);
    //realPerformance<OctupoleMVDRSolver>(scenario, grader);
    //realPerformance<ImplicitMVDRSolver>(scenario, grader);
    realPerformance<QuadrupoleImplicitMVDRSolver>(scenario, grader);
    //realPerformance<OctupoleImplicitMVDRSolver>(scenario, grader);

    //spdlog::info(accuracy<ReverseBarnesHutSolver>(scenario, grader, 0.2));
    //spdlog::info(accuracy<QuadrupoleReverseBarnesHutSolver>(scenario, grader, 0.2));
    //spdlog::info(accuracy<OctupoleReverseBarnesHutSolver>(scenario, grader, 0.11));
    //spdlog::info(accuracy<HexadecupoleReverseBarnesHutSolver>(scenario, grader, 0.2));

    //spdlog::info(accuracy<QuadrupoleImplicitReverseBarnesHutSolver>(scenario, grader, 0.2));
    //spdlog::info(accuracy<OctupoleImplicitReverseBarnesHutSolver>(scenario, grader, 0.2));
    //spdlog::info(accuracy<HexadecupoleImplicitReverseBarnesHutSolver>(scenario, grader, 0.11));
    //spdlog::info(accuracy<TriacontadyupoleImplicitReverseBarnesHutSolver>(scenario, grader, 0.2));

    //spdlog::info(accuracy<MVDRSolver>(scenario, grader, 0.3));
    //spdlog::info(accuracy<QuadrupoleMVDRSolver>(scenario, grader, 0.3));
    //spdlog::info(accuracy<OctupoleMVDRSolver>(scenario, grader, 0.3));

    //spdlog::info(accuracy<ImplicitMVDRSolver>(scenario, grader, 0.4));
    //spdlog::info(accuracy<QuadrupoleImplicitMVDRSolver>(scenario, grader, 0.4));
    //spdlog::info(accuracy<OctupoleImplicitMVDRSolver>(scenario, grader, 0.4));

    //spdlog::info(accuracy<FMMSolver>(scenario, grader, 0.3));
    //spdlog::info(accuracy<QuadrupoleFMMSolver>(scenario, grader, 0.3));
    //spdlog::info(performance<QuadrupoleFMMSolver>(scenario, 1, 0.3).count());
    //spdlog::info(accuracy<OctupoleFMMSolver>(scenario, grader, 0.3));

    //spdlog::info(accuracy<ImplicitFMMSolver>(scenario, grader, 0.2));
    //spdlog::info(accuracy<QuadrupoleImplicitFMMSolver>(scenario, grader, 0.2));
    //spdlog::info(accuracy<OctupoleImplicitFMMSolver>(scenario, grader, 0.2));
    //spdlog::info(accuracy<QuadrupoleFMMSolver>(scenario, grader, 0.2));

    std::vector<std::size_t> nValues{};
    for (int i = 1'000; i < 100'000; i *= 1.5) nValues.emplace_back(i);
//    sweepN<
//            QuadrupoleBarnesHutSolver,
//            QuadrupoleImplicitFMMSolver,
//            QuadrupoleImplicitMVDRSolver
//    >(nValues, 4);

    std::vector<float> thetaValues{};
    for (int i = 1; i < 10; i++) thetaValues.emplace_back((float) i / 10.0f);
    //    sweepTheta<
    //            FMMSolver,
    //            ImplicitFMMSolver
    //    >(scenario, thetaValues);

    //sampleExactField(Generator::trio());

    //    Rule rule = grader.rule();
    //    Simulation simulation;
    //    from_json(scenario, simulation);
    //    QuadrupoleImplicitFMMSolver solver{simulation, rule};
    //    solver.theta() = 0.3566;
    //    spdlog::info("{} s", timedRun(solver, 10).count());
    //    spdlog::info("{} % error", grader.error(simulation));
}
