//
// Created by Jackson Campolattaro on 1/19/23.
//

#ifndef N_BODY_RANDOM_H
#define N_BODY_RANDOM_H

#include <spdlog/spdlog.h>

#include <NBody/Simulation/Simulation.h>

namespace NBody {

    static std::unique_ptr<Simulation> randomSimulation(std::size_t n) {

        spdlog::info("Generating a random scenario with {} particles", n);

        std::uint32_t seed = 42;
        std::mt19937 generator{seed};
        std::uniform_real_distribution<float> positionDistribution{-100.0f, 100.0f};
        std::uniform_real_distribution<float> velocityDistribution{-10.0f, 10.0f};
        std::exponential_distribution<float> massDistribution{1.0f};
        std::uniform_real_distribution<float> colorDistribution{0.3f, 0.9f};

        Physics::Rule gravity{1.0f};

        // Create a simulation with randomized starting conditions
        auto simulation = std::make_unique<Simulation>();
        for (int i = 0; i < n; ++i) {

            Simulation::Particle particle = simulation->newParticle();
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

        return simulation;
    }

}


#endif //N_BODY_RANDOM_H
