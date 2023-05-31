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
