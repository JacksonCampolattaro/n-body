//
// Created by Jackson Campolattaro on 1/8/23.
//

#include "Benchmark/Graders/ConstitutionalGrader.h"
#include "Benchmark/Graders/MeanGrader.h"
#include "Benchmark/Graders/RMSGrader.h"
#include "Generator.h"
#include "field.h"
#include "fieldPlot.h"
#include "benchmark.h"

#include <gtkmm.h>
#include <matplot/matplot.h>

#include <NBody/Physics/Rules/Gravity.h>
#include <NBody/Physics/Rules/SimpleTrackingRule.h>
#include <NBody/Physics/Rules/AdvancedTrackingRule.h>

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
#include <NBody/Simulation/Solvers/ImplicitLinearBVHFMMSolver.h>

using namespace NBody;

template<typename... Solvers>
void sweepN(const std::vector<std::size_t> &nValues, std::size_t iterations) {

    // Open a CSV file and add the header
    std::ofstream file{"benchmarks/sweep-n.csv"};
    file << "n,solver-id,Solver,θ,Time (s)" << std::endl;

    Gravity rule{};

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

    Gravity rule{};

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
float accuracy(json scenario, const NaiveReferenceGrader &grader, float theta = 0.5) {

    // Create a solver
    Gravity rule{grader.rule()};
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
std::chrono::duration<float> realPerformance(json scenario, const Grader &grader, int iterations = 1) {

    // Create a solver
    Gravity rule = grader.rule();
    Simulation simulation;
    from_json(scenario, simulation);
    CandidateSolver solver{simulation, rule};

    // Select a value of theta that can produce the necessary accuracy
    solver.descentCriterion().theta() = grader.optimalTheta<CandidateSolver>();

    // Time the solver
    auto time = timedRun(solver, iterations);
    spdlog::info("{} (θ={}) --> {} s / iteration", solver.name(), solver.theta(), time.count());
    return time;
}

template<typename CandidateSolver>
void approximationRatio(json scenario, const NaiveReferenceGrader &grader) {

    SimpleTrackingRule<Gravity> rule{grader.rule()};
    Simulation simulation;
    from_json(scenario, simulation);
    CandidateSolver solver{simulation, rule};
    solver.theta() = searchTheta<CandidateSolver>(scenario, grader);

    auto time = timedStep(solver);
    spdlog::info("{} (θ={}) --> {} s / iteration", solver.name(), solver.theta(), time.count());
    std::cout << "Interaction counts: " << rule;
    spdlog::info("Approximation ratio: {}",
                 (float) rule.totalCount() / (float) (simulation.particleCount() * simulation.particleCount()));
}

template<typename CandidateSolver>
void approximationTracking(json scenario, const NaiveReferenceGrader &grader) {

    AdvancedTrackingRule<Gravity> rule{"benchmarks/approximation-tracking.csv", grader.rule()};
    Simulation simulation;
    from_json(scenario, simulation);
    CandidateSolver solver{simulation, rule};
    solver.theta() = 0.37;

    auto time = timedStep(solver);
    spdlog::info("{} (θ={}) --> {} s / iteration", solver.name(), solver.theta(), time.count());
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    Glib::init();
    // Limit to 1 thread when debugging
    //tbb::global_control c{tbb::global_control::max_allowed_parallelism, 1};

    json scenario = Generator::realisticGalaxy();
    //json scenario = Generator::trio();
    //json scenario = Generator::createScenario(Generator::uniformRandomVolume, 112'500);

    //MeanGrader grader{scenario};
    //RMSGrader grader{scenario};
    ConstitutionalGrader grader{scenario, Gravity{1.0f}};

    //plotExactField(scenario);
    //plotMomentApproximations(scenario);
    //plotFieldApproximations(scenario);


    //realPerformance<BarnesHutSolver<Gravity>>(scenario, grader);
    realPerformance<QuadrupoleBarnesHutSolver<Gravity>>(scenario, grader);
    //realPerformance<OctupoleBarnesHutSolver<Gravity>>(scenario, grader);
    //realPerformance<HexadecupoleBarnesHutSolver<Gravity>>(scenario, grader);

    //realPerformance<ReverseBarnesHutSolver<Gravity>>(scenario, grader);
    //realPerformance<QuadrupoleReverseBarnesHutSolver<Gravity>>(scenario, grader);
    //realPerformance<OctupoleReverseBarnesHutSolver<Gravity>>(scenario, grader);
    //realPerformance<HexadecupoleReverseBarnesHutSolver<Gravity>>(scenario, grader);

    //realPerformance<QuadrupoleImplicitReverseBarnesHutSolver<Gravity>>(scenario, grader);

    //realPerformance<LinearBVHSolver<Gravity>>(scenario, grader);
    //realPerformance<QuadrupoleLinearBVHSolver<Gravity>>(scenario, grader);
    //realPerformance<OctupoleLinearBVHSolver<Gravity>>(scenario, grader);

    //realPerformance<FMMSolver<Gravity>>(scenario, grader);
    //realPerformance<QuadrupoleFMMSolver<Gravity>>(scenario, grader);
    //realPerformance<OctupoleFMMSolver<Gravity>>(scenario, grader);

    //realPerformance<ImplicitFMMSolver<Gravity>>(scenario, grader);
    realPerformance<QuadrupoleImplicitFMMSolver<Gravity>>(scenario, grader);
    //realPerformance<OctupoleImplicitFMMSolver<Gravity>>(scenario, grader);

    //realPerformance<MVDRSolver<Gravity>>(scenario, grader);
    //realPerformance<QuadrupoleMVDRSolver<Gravity>>(scenario, grader);
    //realPerformance<OctupoleMVDRSolver<Gravity>>(scenario, grader);

    //realPerformance<ImplicitMVDRSolver<Gravity>>(scenario, grader);
    //realPerformance<QuadrupoleImplicitMVDRSolver<Gravity>>(scenario, grader);
    //realPerformance<OctupoleImplicitMVDRSolver<Gravity>>(scenario, grader);

    //realPerformance<QuadrupoleImplicitLinearBVHFMMSolver<Gravity>>(scenario, grader);

    //spdlog::info(accuracy<ReverseBarnesHutSolver<Gravity>>(scenario, grader, 0.2));
    //spdlog::info(accuracy<QuadrupoleReverseBarnesHutSolver<Gravity>>(scenario, grader, 0.2));
    //spdlog::info(accuracy<OctupoleReverseBarnesHutSolver<Gravity>>(scenario, grader, 0.11));
    //spdlog::info(accuracy<HexadecupoleReverseBarnesHutSolver<Gravity>>(scenario, grader, 0.2));

    //spdlog::info(accuracy<QuadrupoleImplicitReverseBarnesHutSolver<Gravity>>(scenario, grader, 0.2));
    //spdlog::info(accuracy<OctupoleImplicitReverseBarnesHutSolver<Gravity>>(scenario, grader, 0.2));
    //spdlog::info(accuracy<HexadecupoleImplicitReverseBarnesHutSolver<Gravity>>(scenario, grader, 0.11));
    //spdlog::info(accuracy<TriacontadyupoleImplicitReverseBarnesHutSolver<Gravity>>(scenario, grader, 0.2));

    //spdlog::info(accuracy<MVDRSolver<Gravity>>(scenario, grader, 0.3));
    //spdlog::info(accuracy<QuadrupoleMVDRSolver<Gravity>>(scenario, grader, 0.3));
    //spdlog::info(accuracy<OctupoleMVDRSolver<Gravity>>(scenario, grader, 0.3));

    //spdlog::info(accuracy<ImplicitMVDRSolver<Gravity>>(scenario, grader, 0.4));
    //spdlog::info(accuracy<QuadrupoleImplicitMVDRSolver<Gravity>>(scenario, grader, 0.5));
    //spdlog::info(accuracy<OctupoleImplicitMVDRSolver<Gravity>>(scenario, grader, 0.4));

    //spdlog::info(accuracy<FMMSolver<Gravity>>(scenario, grader, 0.33));
    //spdlog::info(accuracy<QuadrupoleFMMSolver<Gravity>>(scenario, grader, 0.3));
    //spdlog::info(performance<QuadrupoleFMMSolver<Gravity>>(scenario, 1, 0.3).count());
    //spdlog::info(accuracy<OctupoleFMMSolver<Gravity>>(scenario, grader, 0.3));

    //spdlog::info(accuracy<ImplicitFMMSolver<Gravity>>(scenario, grader, 0.2));
    //spdlog::info(accuracy<QuadrupoleImplicitFMMSolver<Gravity>>(scenario, grader, 0.2));
    //spdlog::info(accuracy<OctupoleImplicitFMMSolver<Gravity>>(scenario, grader, 0.2));
    //spdlog::info(accuracy<QuadrupoleFMMSolver<Gravity>>(scenario, grader, 0.2));

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

    //approximationRatio<QuadrupoleBarnesHutSolver<SimpleTrackingRule<Gravity>>>(scenario, grader);
    //approximationRatio<QuadrupoleFMMSolver<SimpleTrackingRule<Gravity>>>(scenario, grader);
    //approximationRatio<QuadrupoleMVDRSolver<SimpleTrackingRule<Gravity>>>(scenario, grader);
    //    approximationRatio<QuadrupoleImplicitFMMSolver<SimpleTrackingRule<Gravity>>>(scenario, grader);
    //    approximationRatio<QuadrupoleImplicitMVDRSolver<SimpleTrackingRule<Gravity>>>(scenario, grader);

    //approximationTracking<QuadrupoleBarnesHutSolver<AdvancedTrackingRule<Gravity>>>(scenario, grader);
    //approximationTracking<QuadrupoleMVDRSolver<AdvancedTrackingRule<Gravity>>>(scenario, grader);

    //    Gravity rule = grader.rule();
    //    Simulation simulation;
    //    from_json(scenario, simulation);
    //    QuadrupoleImplicitFMMSolver solver{simulation, rule};
    //    solver.theta() = 0.3566;
    //    spdlog::info("{} s", timedRun(solver, 10).count());
    //    spdlog::info("{} % error", grader.error(simulation));
}
