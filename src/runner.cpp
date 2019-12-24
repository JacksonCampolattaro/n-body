//
// Created by jackcamp on 12/22/19.
//

#include "runner.h"

int runner::headless(BodyList bodies, PhysicsContext physicsContext, unsigned int cycles) {


    // Creating the solver, and configuring it
    Solver *solver = new BarnesHutSolver();
    solver->enableThreading();

    // Start the timer
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Looping to run the simulation for many cycles
    spdlog::get("log")->info("Starting simulation with {} cycles", cycles);
    for (int i = 0; i < cycles; ++i) {
        spdlog::get("log")->debug("Starting cycle {} ({}% Complete)",
                                  i,
                                  100.0f * (float) i / (float) cycles);
        solver->solve(&bodies, &physicsContext);
    }

    // End the timer
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // Output the elapsed time
    spdlog::get("log")->info("Completed {} cycles in {} s",
                             cycles,
                             std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0f
    );
    spdlog::get("log")->info("Averaged {} µs per cycle",
                             std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() /
                             (float) cycles
    );

    return 0;
}
