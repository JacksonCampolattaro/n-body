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
    Glib::init();

    std::size_t repetitions = 5;
    std::size_t nMax = 300'000;

    std::ofstream out{"benchmarks/tree-construction-random-data.csv"};
    out << "Tree,N,Max Leaf Size,Time\n";

    for (std::size_t n = 100; n < nMax; n = n * 1.5) {
        spdlog::info("Running benchmarks with {} random particles", n);

        for (int i = 0; i < repetitions; ++i) {

            spdlog::info("Generating dataset, {} / {}", i + 1, repetitions);
            auto scenario = Generator::createScenario(Generator::uniformRandomVolume, n);

            {
                spdlog::info("Linear-BVH construction");
                Simulation simulation;
                from_json(scenario, simulation);
                testTreeConstruction<ActiveLinearBVH>("Linear BVH", simulation, out);
            }

            {
                spdlog::info("Octree construction");
                Simulation simulation;
                from_json(scenario, simulation);
                testTreeConstruction<ActiveOctree>("Octree", simulation, out);
            }
        }
    }

}
