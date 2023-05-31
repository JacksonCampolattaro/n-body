//
// Created by Jackson Campolattaro on 5/31/23.
//

#ifndef N_BODY_BENCHMARK_H
#define N_BODY_BENCHMARK_H

#include <spdlog/spdlog.h>
#include <boost/progress.hpp>

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

    spdlog::debug("Running solver \"{}\" for {} iteration(s)", solver.name(), iterations);
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


#endif //N_BODY_BENCHMARK_H
