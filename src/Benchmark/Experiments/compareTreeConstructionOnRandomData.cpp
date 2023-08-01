//
// Created by Jackson Campolattaro on 6/2/23.
//

#include <spdlog/spdlog.h>

#include <NBody/Simulation/Simulation.h>

#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>

#include "../Generator.h"
#include "../benchmark.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    std::size_t repetitions = 3;
    std::size_t nMax = 10'000'000;

    std::ofstream out{argc > 1 ? std::string{argv[1]} : "benchmarks/tree-construction-random-data.csv"};
    out << "Tree,N,Max Leaf Size,Time\n";

    for (std::size_t n = 1'000; n < nMax; n = n * 1.25f) {
        spdlog::info("Running benchmarks with {} random particles", n);

        Simulation scenario = Generator::perlinNoiseRandomVolume(n);

        for (int i = 0; i < repetitions; ++i) {
            spdlog::info("{} / {}", i + 1, repetitions);

            {
                spdlog::info("Linear-BVH construction");
                Simulation simulation{scenario};
                testTreeConstruction<ActiveLinearBVH>("Linear BVH", simulation, out);
            }

            {
                spdlog::info("Octree construction");
                Simulation simulation{scenario};
                testTreeConstruction<ActiveOctree>("Octree", simulation, out);
            }
        }
    }

}
