//
// Created by Jackson Campolattaro on 1/19/23.
//

#ifndef N_BODY_RANDOM_H
#define N_BODY_RANDOM_H

#include <spdlog/spdlog.h>

#include <NBody/Simulation/Simulation.h>

namespace NBody {

    static json randomVolumeSimulation(std::size_t n) {

        spdlog::info("Generating a random scenario with {} particles", n);

        std::uint32_t seed = 42;
        std::mt19937 generator{seed};
        std::uniform_real_distribution<float> positionDistribution{-10.0f, 10.0f};
        std::uniform_real_distribution<float> velocityDistribution{-0.01f, 0.01f};
        std::exponential_distribution<float> massDistribution{1.0f};
        std::uniform_real_distribution<float> colorDistribution{0.3f, 0.9f};

        // Create a simulation with randomized starting conditions
        Simulation simulation;
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
            particle.setMass(massDistribution(generator) / (float) n);
            particle.setSphere({std::cbrt(particle.get<Physics::Mass>().mass())});
            particle.emplace<Physics::ActiveTag>();
            particle.emplace<Physics::PassiveTag>();
        }

        std::ofstream referenceFile{"test.json"};
        json scenario;
        to_json(scenario, simulation);
        referenceFile << std::setw(4) << scenario;
        referenceFile.close();

        return scenario;
    }

    static json randomGalaxySimulation(std::size_t n) {

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

        Simulation simulation;

        // Add the "sun" particle
        Simulation::Particle sun = simulation.newParticle();
        sun.setPosition({0.0f, 0.0f, 0.0f});
        sun.setVelocity({0.0f, 0.0f, 0.0f});
        sun.setColor({0.9f, 0.9f, 0.8f});
        sun.setMass(sunMassFraction);
        sun.setSphere({std::cbrt(sun.get<Physics::Mass>().mass())});
        sun.emplace<Physics::ActiveTag>();
        sun.emplace<Physics::PassiveTag>();

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
            particle.setMass(std::max(massDistribution(generator) / (float) n, (0.0001f / (float) n)));
            particle.setSphere({std::cbrt(particle.get<Physics::Mass>().mass())});
            particle.emplace<Physics::ActiveTag>();
            particle.emplace<Physics::PassiveTag>();
        }

        std::ofstream referenceFile{"test.json"};
        json scenario;
        to_json(scenario, simulation);
        referenceFile << std::setw(4) << scenario;
        referenceFile.close();

        return scenario;
    }

}


#endif //N_BODY_RANDOM_H
