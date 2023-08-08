//
// Created by Jackson Campolattaro on 8/6/23.
//

#include <spdlog/spdlog.h>

#include <NBody/Physics/Rules/Gravity.h>
#include <NBody/Physics/Rules/SimpleTrackingRule.h>

#include <NBody/Simulation/Simulation.h>

#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/ImplicitReverseBarnesHutSolver.h>
#include <NBody/Simulation/Solvers/LinearBVHSolver.h>
#include <NBody/Simulation/Solvers/ImplicitFMMSolver.h>
#include <NBody/Simulation/Solvers/ImplicitMVDRSolver.h>

#include "../Generator.h"
#include "../bestTheta.h"
#include "../benchmark.h"

template<typename CandidateSolver>
void runThetaTests(std::string label, std::ostream &out, const Grader &grader, float minTheta, float maxTheta) {

    int steps = 100;
    for (int i = 0; i < steps; ++i) {
        float theta = std::lerp(minTheta, maxTheta, sqrt((float) i / (float) steps));

        auto rule = grader.rule();
        auto simulation = grader.scenario();
        CandidateSolver solver{simulation, rule};
        solver.descentCriterion().theta() = theta;
        auto time = timedInvoke([&]() {
            solver.step();
        });
        float error = grader.error(simulation);

        out << label << ","
            << simulation.size() << ","
            << theta << ","
            << time.count() << ","
            << error << std::endl;

        spdlog::info("{} (theta = {}) --> {}s, {}%", label, theta, time.count(), error);

    }
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);


    ConstitutionalGrader grader{std::filesystem::path{"../n-body-scenarios/benchmark/LOW.bin"}};

//    {
//        std::ofstream out{"benchmarks/bh-theta.csv"};
//        out << "Solver,Multipole Order,N,θ,Time (s),% Error (Constitutional)\n";
//        runThetaTests<QuadrupoleBarnesHutSolver<Gravity>>("BH,Quadrupole", out, grader, 0.1, 0.7);
//        runThetaTests<OctupoleBarnesHutSolver<Gravity>>("BH,Octupole", out, grader, 0.2, 0.8);
//        runThetaTests<HexadecupoleBarnesHutSolver<Gravity>>("BH,Hexadecupole", out, grader, 0.25, 0.85);
//    }

    {
        std::ofstream out{"benchmarks/lbvh-theta.csv"};
        out << "Solver,Multipole Order,N,θ,Time (s),% Error (Constitutional)\n";
        runThetaTests<QuadrupoleLinearBVHSolver<Gravity>>("LBVH,Quadrupole", out, grader, 0.3, 1.5);
        runThetaTests<OctupoleLinearBVHSolver<Gravity>>("LBVH,Octupole", out, grader, 0.3, 1.5);
        runThetaTests<HexadecupoleLinearBVHSolver<Gravity>>("LBVH,Hexadecupole", out, grader, 0.3, 1.5);
    }

    //    {
    //        std::ofstream out{"benchmarks/fmm-theta.csv"};
    //        out << "Solver,Multipole Order,N,θ,Time (s),% Error (Constitutional)\n";
    //        runThetaTests<QuadrupoleImplicitFMMSolver<Gravity>>("FMM,Quadrupole", out, grader, 0.1, 0.7);
    //        runThetaTests<OctupoleImplicitFMMSolver<Gravity>>("FMM,Octupole", out, grader, 0.2, 0.8);
    //        runThetaTests<HexadecupoleImplicitFMMSolver<Gravity>>("FMM,Hexadecupole", out, grader, 0.25, 0.85);
    //    }

    //    {
    //        std::ofstream out{"benchmarks/mvdr-theta.csv"};
    //        out << "Solver,Multipole Order,N,θ,Time (s),% Error (Constitutional)\n";
    //        runThetaTests<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR,Quadrupole", out, grader, 0.3, 1.5);
    //        runThetaTests<OctupoleImplicitMVDRSolver<Gravity>>("MVDR,Octupole", out, grader, 0.3, 1.5);
    //        runThetaTests<HexadecupoleImplicitMVDRSolver<Gravity>>("MVDR,Hexadecupole", out, grader, 0.3, 1.5);
    //    }

}
