//
// Created by Jackson Campolattaro on 1/8/23.
//

#include <random>
#include <gtkmm.h>

#include <spdlog/spdlog.h>

#include <boost/progress.hpp>

#include <NBody/Physics/Rule.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/NaiveSolver.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>
#include <NBody/Simulation/Solvers/DualTreeSolver.h>

using namespace NBody;

float L2Norm(const Simulation &a, const Simulation &b) {
    auto aValues = a.view<const Position, const Mass>();
    auto bValues = b.view<const Position, const Mass>();
    return std::transform_reduce(aValues.begin(), aValues.end(), bValues.begin(), 0.0f,
                                 std::plus{},
                                 [&](const auto &x, const auto &y) {
                                     return glm::distance2(
                                             (glm::vec3) a.get<Position>(x),
                                             (glm::vec3) b.get<Position>(y)
                                     );
                                 }
    ) / (float) std::distance(aValues.begin(), aValues.end());
}

float weightedL2Norm(const Simulation &a, const Simulation &b) {
    auto aValues = a.view<const Position, const Mass>();
    auto bValues = b.view<const Position, const Mass>();
    return std::transform_reduce(aValues.begin(), aValues.end(), bValues.begin(), 0.0f,
                                 std::plus{},
                                 [&](const auto &x, const auto &y) {
                                     return glm::distance2(
                                             (glm::vec3) a.get<Position>(x),
                                             (glm::vec3) b.get<Position>(y)
                                     ) * std::pow(a.get<Mass>(x).mass(), 2.0f);
                                 }
    ) / (float) std::distance(aValues.begin(), aValues.end());
}

float maxSquaredError(const Simulation &a, const Simulation &b) {
    auto aValues = a.view<const Position, const Mass>();
    auto bValues = b.view<const Position, const Mass>();
    return std::transform_reduce(aValues.begin(), aValues.end(), bValues.begin(), 0.0f,
                                 [&](float x, float y) {
                                     return std::max(x, y);
                                 },
                                 [&](const auto &x, const auto &y) {
                                     return glm::distance2(
                                             (glm::vec3) a.get<Position>(x),
                                             (glm::vec3) b.get<Position>(y)
                                     );
                                 }
    );
}

template<typename SolverType>
std::chrono::duration<float> timedRun(Simulation &simulation, Physics::Rule &rule, std::size_t iterations) {
    boost::progress_display display(iterations);
    SolverType solver{simulation, rule};
    auto startTime = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        solver.step();
        ++display;
    }
    auto finishTime = std::chrono::steady_clock::now();
    std::cout << std::endl;
    return (finishTime - startTime) / iterations;
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    Glib::init();

    // Test parameters
    std::size_t N = 1'000;
    std::size_t iterations = 1'000;
    std::uint32_t seed = 42;

    std::mt19937 generator{seed};
    std::uniform_real_distribution<float> positionDistribution{-100.0f, 100.0f};
    std::uniform_real_distribution<float> velocityDistribution{-10.0f, 10.0f};
    std::exponential_distribution<float> massDistribution{1.0f};
    std::uniform_real_distribution<float> colorDistribution{0.3f, 0.9f};

    Physics::Rule gravity{1.0f};

    // Create a simulation with randomized starting conditions
    Simulation simulation;
    for (int i = 0; i < N; ++i) {

        Simulation::Particle particle = simulation.newParticle();
        particle.setPosition({positionDistribution(generator),
                              positionDistribution(generator),
                              positionDistribution(generator)});
        particle.setVelocity({velocityDistribution(generator),
                              velocityDistribution(generator),
                              velocityDistribution(generator)});
        particle.setColor({colorDistribution(generator),
                           colorDistribution(generator),
                           colorDistribution(generator)});
        particle.setMass(massDistribution(generator));
        particle.setSphere({std::cbrt(particle.get<Physics::Mass>().mass())});
        particle.emplace<Physics::ActiveTag>();
        particle.emplace<Physics::PassiveTag>();

    }

    // Save the initial conditions
    std::ofstream referenceFile{"test.json"};
    json j;
    to_json(j, simulation);
    referenceFile << std::setw(4) << j;
    referenceFile.close();

    // Create reference & barnesHut simulations by loading the initial conditions
    Simulation initial;
    from_json(j, initial);
    Simulation reference;
    from_json(j, reference);
    Simulation barnesHut;
    from_json(j, barnesHut);


    // Run a reference simulation with the naive solver
    spdlog::info("Performing Naive Simulation");
    auto naiveTime = timedRun<NaiveSolver>(reference, gravity, iterations);

    // Run an approximate simulation using another solver
    spdlog::info("Performing Barnes-Hut Simulation");
    auto barnesHutTime = timedRun<BarnesHutSolver>(barnesHut, gravity, iterations);

    // Save both simulations for later reference
    std::ofstream naiveFile{"test-naive.json"};
    naiveFile << reference;
    naiveFile.close();
    std::ofstream barnesHutFile{"test-bh.json"};
    barnesHutFile << barnesHut;
    barnesHutFile.close();

    // Determine the difference between the initial conditions and the reference simulation
    spdlog::info("L2(naive, init) = {}", L2Norm(initial, reference));
    spdlog::info("wL2(naive, init) = {}", weightedL2Norm(initial, reference));
    spdlog::info("max^2(naive, init) = {}", maxSquaredError(initial, reference));

    // Determine the difference between each simulation and the reference
    spdlog::info("L2(naive, bh) = {}", L2Norm(reference, barnesHut));
    spdlog::info("wL2(naive, bh) = {}", weightedL2Norm(reference, barnesHut));
    spdlog::info("max^2(naive, bh) = {}", maxSquaredError(reference, barnesHut));

    // Print time & accuracy results
    spdlog::info("time(naive) = {}", naiveTime.count());
    spdlog::info("time(bh) = {}", barnesHutTime.count());
}
