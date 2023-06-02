//
// Created by Jackson Campolattaro on 6/2/23.
//

#include <gtkmm.h>
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

template<typename TreeType>
void testTreeConstruction(const std::string &label, Simulation &simulation, std::ofstream &out) {
    TreeType tree{simulation};

    auto time = timedInvoke([&]() { tree.build(); });

    out << label << ","
        << simulation.particleCount() << ","
        << tree.maxLeafSize() << ","
        << time.count() << "\n";
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    Glib::init();

    std::size_t repetitions = 10;

    std::ofstream out{"benchmarks/tree-construction.csv"};
    out << "Tree,N,Max Leaf Size,Time\n";

    for (const std::string &dataset: {"LOW", "MED", "HI"}) {

        std::ifstream scenarioFile{fmt::format("../n-body-scenarios/benchmark/{}.bin", dataset)};

        spdlog::info("Benchmarking Linear-BVH construction, AGORA {}", dataset);
        for (int i = 0; i < repetitions; ++i) {
            spdlog::info("{} / {}", i + 1, repetitions);
            Simulation simulation;
            from_tipsy(scenarioFile, simulation);
            testTreeConstruction<ActiveLinearBVH>("Linear BVH", simulation, out);
        }

        spdlog::info("Benchmarking Octree construction, AGORA {}", dataset);
        for (int i = 0; i < repetitions; ++i) {
            spdlog::info("{} / {}", i + 1, repetitions);
            Simulation simulation;
            from_tipsy(scenarioFile, simulation);
            testTreeConstruction<ActiveOctree>("Octree", simulation, out);
        }
    }

}
