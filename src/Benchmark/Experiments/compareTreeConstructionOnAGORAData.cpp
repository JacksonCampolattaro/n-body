//
// Created by Jackson Campolattaro on 6/2/23.
//

#include <gtkmm.h>
#include <spdlog/spdlog.h>

#include <NBody/Simulation/Simulation.h>

#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>

#include "../Generator.h"
#include "../benchmark.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    std::size_t repetitions = 10;

    std::ofstream out{argc > 1 ? std::string{argv[1]} : "benchmarks/tree-construction-agora-data.csv"};
    out << "Tree,N,Max Leaf Size,Node Count,Time\n";

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
