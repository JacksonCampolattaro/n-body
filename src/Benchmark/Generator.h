//
// Created by Jackson Campolattaro on 1/27/23.
//

#ifndef N_BODY_GENERATOR_H
#define N_BODY_GENERATOR_H

#include <random>

#include <spdlog/spdlog.h>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>

namespace NBody::Generator {

    static Simulation &uniformRandomVolume(Simulation &simulation, std::size_t n) {
        spdlog::info("Generating a random scenario with {} particles", n);

        std::uint32_t seed = 42;
        std::mt19937 generator{seed};
        std::uniform_real_distribution<float> positionDistribution{0.0f, 10.0f * std::cbrt((float) n)};
        std::uniform_real_distribution<float> velocityDistribution{-std::cbrt((float) n) / 10,
                                                                   std::cbrt((float) n) / 10};
        std::exponential_distribution<float> massDistribution{1.0f};
        std::uniform_real_distribution<float> colorDistribution{0.3f, 0.9f};

        // Create a simulation with randomized starting conditions
        for (int i = 0; i < n; ++i) {

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
            particle.setAcceleration({0.0f, 0.0f, 0.0f});
            particle.setSphere({std::cbrt(particle.get<Physics::Mass>().mass())});
        }

        return simulation;
    }

    static Simulation &galaxy(Simulation &simulation, std::size_t n) {
        spdlog::info("Generating a random galaxy scenario with {} particles", n);

        std::uint32_t seed = 42;
        std::mt19937 generator{seed};

        float minimumDistance = 10.0f;//(float) n / 10000.0f;
        float sunMassFraction = 0.5f;
        float orbitingMassFraction = 1.0f - sunMassFraction;

        std::uniform_real_distribution<float> deviationDistribution{-0.1f, 0.1f};
        std::uniform_real_distribution<float> angleDistribution{0.0f, 2 * M_PI};
        std::uniform_real_distribution<float> distanceDistribution{minimumDistance, minimumDistance + 50.0f};
        std::exponential_distribution<float> massDistribution{orbitingMassFraction};
        std::uniform_real_distribution<float> colorDistribution{0.1f, 0.9f};

        // Add the "sun" particle
        Simulation::Particle sun = simulation.newParticle();
        sun.setPosition({0.0f, 0.0f, 0.0f});
        sun.setVelocity({0.0f, 0.0f, 0.0f});
        sun.setColor({0.9f, 0.9f, 0.8f});
        sun.setMass(sunMassFraction);
        sun.setAcceleration({0.0f, 0.0f, 0.0f});
        sun.setSphere({std::cbrt(sun.get<Physics::Mass>().mass())});

        // Add all orbiting particles
        for (int i = 0; i < n - 1; ++i) {

            auto angle = angleDistribution(generator);
            auto orientation = glm::normalize(glm::vec3{
                    std::cos(angle),
                    deviationDistribution(generator),
                    std::sin(angle)
            });
            float distance = distanceDistribution(generator);

            auto velocity = glm::normalize(-glm::vec3{
                    std::cos(angle + (M_PI / 4.0f)),
                    deviationDistribution(generator),
                    std::sin(angle + (M_PI / 4.0f))
            }) * std::sqrt(sunMassFraction / distance);

            Simulation::Particle particle = simulation.newParticle();
            particle.setPosition({orientation * distance});
            particle.setVelocity({velocity});
            particle.setColor({colorDistribution(generator),
                               colorDistribution(generator),
                               colorDistribution(generator)});
            particle.setMass(massDistribution(generator));
            particle.setAcceleration({0.0f, 0.0f, 0.0f});
            particle.setSphere({std::cbrt(particle.get<Physics::Mass>().mass())});
        }

        return simulation;
    }

    static Simulation &realisticGalaxy(Simulation &simulation, std::size_t n) {
        spdlog::info("Reading a realistic galaxy scenario from a file");
        std::ifstream inputFile("/Users/jackcamp/Documents/n-body-scenarios/benchmark/LOW.bin", std::ios::binary);
        from_tipsy(inputFile, simulation);
        return simulation;
    }

    static Simulation &bake(Simulation &simulation, std::size_t iterations) {
        spdlog::info("\"Baking\" the simulation for {} iterations", iterations);
        Rule rule{};
        BarnesHutSolver solver{simulation, rule};
        for (int i = 0; i < iterations; ++i) solver.step();

        return simulation;
    }

    static json createScenario(const std::function<Simulation &(Simulation &, std::size_t)> &generator,
                               std::size_t n, std::size_t iterations) {
        Simulation simulation;
        bake(generator(simulation, n), iterations);

        json scenario;
        to_json(scenario, simulation);

        std::ofstream referenceFile{"test.json"};
        referenceFile << std::setw(4) << scenario;
        referenceFile.close();

        return scenario;
    }
}

#endif //N_BODY_GENERATOR_H
