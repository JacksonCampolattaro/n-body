//
// Created by Jackson Campolattaro on 1/27/23.
//

#ifndef N_BODY_GENERATOR_H
#define N_BODY_GENERATOR_H

#include <random>

#include <spdlog/spdlog.h>
#include <glm/gtc/noise.hpp>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solvers/BarnesHutSolver.h>

namespace NBody::Generator {

    static Simulation &perlinNoiseRandomVolume(Simulation &simulation, std::size_t n) {
        spdlog::debug("Generating a perlin-noise scenario with {} particles", n);

        std::uint32_t seed = 42;
        std::mt19937 generator{seed};
        std::uniform_real_distribution<float> positionDistribution{0.0f, 10.0f * std::cbrt((float) n)};
        std::uniform_real_distribution<float> velocityDistribution{-std::cbrt((float) n) / 10,
                                                                   std::cbrt((float) n) / 10};
        std::exponential_distribution<float> massDistribution{1.0f};
        std::uniform_real_distribution<float> colorDistribution{0.3f, 0.9f};

        while (simulation.particleCount() < n) {

            glm::vec3 position{positionDistribution(generator),
                               positionDistribution(generator),
                               positionDistribution(generator)};

            auto perlin = glm::perlin(position / 256.0f) + glm::perlin(position);
            if (perlin > 0.9) {

                auto particle = simulation.newParticle()
                        .setPosition(position)
                        .setVelocity({velocityDistribution(generator),
                                      velocityDistribution(generator),
                                      velocityDistribution(generator)})
                        .setMass(massDistribution(generator) * (perlin - 0.5f) * 4.0f);

                particle
                        .setSphere({std::cbrt(particle.get<Physics::Mass>().mass())})
                        .setColor({colorDistribution(generator),
                                   colorDistribution(generator),
                                   colorDistribution(generator)});

                // Marks the last added particle in red (useful for debugging)
                if (simulation.particleCount() == 31343)
                    particle.setColor({1.0, 0.0, 0.0});
            }

        }

        return simulation;
    }

    static Simulation &uniformRandomVolume(Simulation &simulation, std::size_t n) {
        spdlog::debug("Generating a random scenario with {} particles", n);

        std::uint32_t seed = 42;
        std::mt19937 generator{seed};
        std::uniform_real_distribution<float> positionDistribution{0.0f, 10.0f/* * std::cbrt((float) n / 10000)*/};
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
            particle.setMass(massDistribution(generator) / (float) n);
            particle.setAcceleration({0.0f, 0.0f, 0.0f});
            particle.setSphere({std::cbrt(particle.get<Physics::Mass>().mass())});
            particle.setColor({colorDistribution(generator),
                               colorDistribution(generator),
                               colorDistribution(generator)});
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
        sun.setMass(sunMassFraction);
        sun.setAcceleration({0.0f, 0.0f, 0.0f});
        sun.setSphere({std::cbrt(sun.get<Physics::Mass>().mass())});
        sun.setColor({0.9f, 0.9f, 0.8f});

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
            particle.setMass(std::max(massDistribution(generator) / (float) n, (0.0001f / (float) n)));
            particle.setAcceleration({0.0f, 0.0f, 0.0f});
            particle.setSphere({std::cbrt(particle.get<Physics::Mass>().mass())});
            particle.setColor({colorDistribution(generator),
                               colorDistribution(generator),
                               colorDistribution(generator)});
        }

        return simulation;
    }

    static json realisticGalaxy() {
        Simulation s;
        std::ifstream file{"../n-body-scenarios/benchmark/LOW.bin"};
        from_tipsy(file, s);
        json scenario;
        to_json(scenario, s);
        return scenario;
    }


    static json trio() {

        Simulation s;
        s.newParticle().setPosition({0.0f, 2.0f, 0.0f}).setMass(2.0f);
        s.newParticle().setPosition({-1.0f, -1.0f, 0.0f}).setMass(1.0f);
        s.newParticle().setPosition({1.0f, -1.0f, 0.0f}).setMass(1.0f);

        json scenario;
        to_json(scenario, s);
        return scenario;
    }

    static Simulation fromFile(const std::filesystem::path &path) {
        Simulation simulation;
        std::ifstream file{path};
        if (path.extension().string() == ".json") {
            nlohmann::json jsonScenario;
            file >> jsonScenario;
            from_json(jsonScenario, simulation);
        } else if (path.extension().string() == ".bin") {
            from_tipsy(file, simulation);
            spdlog::debug("loaded {} particles", simulation.particleCount());
        } else {
            spdlog::error("Unrecognized file extension");
            exit(1);
        }
        return simulation;
    }

    static Simulation createScenario(const std::function<Simulation &(Simulation &, std::size_t)> &generator,
                                     std::size_t n) {
        Simulation simulation;
        generator(simulation, n);
        return simulation;
    }
}

#endif //N_BODY_GENERATOR_H
