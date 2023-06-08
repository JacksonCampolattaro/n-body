//
// Created by Jackson Campolattaro on 6/8/23.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "NBody/Physics/Summaries/CenterOfMassSummary.h"
#include "NBody/Physics/Rules/Gravity.h"
#include <NBody/Simulation/Simulation.h>

TEST_CASE("Center of mass interactions should be accurate at a reasonable distance", "[CenterOfMassSummary]") {

    NBody::Simulation simulation;
    simulation.newParticle()
            .setPosition({0, 0, -1})
            .setMass(1.0f);
    simulation.newParticle()
            .setPosition({0, 0, 1})
            .setMass(1.0f);
    auto activeEntities = simulation.validEntities();

    NBody::CenterOfMassSummary summary;
    summary.summarize(activeEntities, simulation);

    Gravity rule{};

    Position passivePosition{0, 20, 0};

    auto actualAcceleration = std::transform_reduce(
            activeEntities.begin(), activeEntities.end(),
            Physics::Acceleration{}, std::plus{},
            [&](auto activeEntity) {
                return rule(simulation.get<const Position>(activeEntity),
                            simulation.get<const Mass>(activeEntity),
                            passivePosition);
            }
    );
    auto approximateAcceleration = rule(summary.centerOfMass(), summary.totalMass(), passivePosition);

    float error = glm::distance((glm::vec3) actualAcceleration, approximateAcceleration);
    float relativeError = error / glm::length((glm::vec3) actualAcceleration);

    CAPTURE(actualAcceleration);
    CAPTURE(approximateAcceleration);
    REQUIRE(relativeError < 0.005);
}
