//
// Created by Jackson Campolattaro on 6/8/23.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "NBody/Physics/Summaries/MultipoleMassSummary.h"
#include "NBody/Physics/Rules/Gravity.h"
#include <NBody/Simulation/Simulation.h>

TEST_CASE("Multipole moment interactions should be progressively more accurate", "[MultipoleMassSummary]") {

    NBody::Simulation simulation;
    simulation.newParticle()
            .setPosition({0, 0, -1})
            .setMass(1.0f);
    simulation.newParticle()
            .setPosition({0, 0, 1})
            .setMass(1.0f);
    simulation.newParticle()
            .setPosition({0, 3, 1})
            .setMass(2.0f);
    auto activeEntities = simulation.validEntities();

    NBody::QuadrupoleMassSummary quadrupoleMassSummary;
    quadrupoleMassSummary.summarize(activeEntities, simulation);
    NBody::OctupoleMassSummary octupoleMassSummary;
    octupoleMassSummary.summarize(activeEntities, simulation);
    NBody::HexadecupoleMassSummary hexadecupoleMassSummary;
    hexadecupoleMassSummary.summarize(activeEntities, simulation);

    Gravity rule{};
    Position passivePosition{0, 10, 0};

    auto actualAcceleration = std::transform_reduce(
            activeEntities.begin(), activeEntities.end(),
            Physics::Acceleration{}, std::plus{},
            [&](auto activeEntity) {
                return rule(simulation.get<const Position>(activeEntity),
                            simulation.get<const Mass>(activeEntity),
                            passivePosition);
            }
    );
    Acceleration quadrupoleAcceleration = rule(quadrupoleMassSummary.centerOfMass(), quadrupoleMassSummary,
                                               passivePosition);
    float quadrupoleError = glm::distance((glm::vec3) actualAcceleration, quadrupoleAcceleration);
    float quadrupoleRelativeError = quadrupoleError / glm::length((glm::vec3) actualAcceleration);
    CAPTURE(quadrupoleAcceleration);

    Acceleration octupoleAcceleration = rule(octupoleMassSummary.centerOfMass(), octupoleMassSummary,
                                             passivePosition);
    float octupoleError = glm::distance((glm::vec3) actualAcceleration, octupoleAcceleration);
    float octupoleRelativeError = octupoleError / glm::length((glm::vec3) actualAcceleration);
    CAPTURE(octupoleAcceleration);

    Acceleration hexadecupoleAcceleration = rule(hexadecupoleMassSummary.centerOfMass(), hexadecupoleMassSummary,
                                                 passivePosition);
    float hexadecupoleError = glm::distance((glm::vec3) actualAcceleration, hexadecupoleAcceleration);
    float hexadecupoleRelativeError = hexadecupoleError / glm::length((glm::vec3) actualAcceleration);
    CAPTURE(hexadecupoleAcceleration);

    REQUIRE(quadrupoleError > octupoleError);
    REQUIRE(octupoleError > hexadecupoleError);

    REQUIRE(hexadecupoleError <= 0.005);
}
