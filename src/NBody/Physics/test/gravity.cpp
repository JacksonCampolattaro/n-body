
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include <glm/gtx/string_cast.hpp>

#include <NBody/Physics/Rule.h>

using namespace NBody::Physics;

TEST_CASE("Particle-particle acceleration should be accurate", "[Gravity]") {

    Position pA{0, 0, 0};
    Mass mA{2.0};

    Position pB{2, 0, 0};

    Rule G{2.0};

    auto a = G(pA, mA, pB);

    CAPTURE(a);
    CHECK(glm::length(a - glm::vec3{-1.0f, 0.0f, 0.0f}) < 0.0005f);
}

TEST_CASE("Quadrupole acceleration should be accurate at the evaluation point", "[Gravity]") {

    Position pA{0, 0, 0};
    Mass mA{2.0};

    Position pP{2, 0, 0};

    Rule G{2.0};

    auto vectorAcceleration = G(pA, mA, pP);
    auto quadrupoleAcceleration = G(pA, mA, pP, NBody::QuadrupoleAccelerationSummary{});

    CHECK(vectorAcceleration == quadrupoleAcceleration.acceleration());
}

TEST_CASE("Quadrupole acceleration should account for small offsets from the evaluation point", "[Gravity]") {

    Position pA{0, 0, 0};
    Mass mA{1.0};

    Position pP{2, 0, 0};

    Rule G{};

    auto monopoleAcceleration = G(pA, mA, pP);
    auto quadrupoleAcceleration = G(pA, mA, pP, NBody::QuadrupoleAccelerationSummary{});

    glm::vec3 offset{
            GENERATE(0.01, 0.1, -0.1, 0.5),
            GENERATE(0.01, 0.1, -0.1),
            GENERATE(0.01, 0.1, -0.1),
    };

    auto realOffsetAcceleration = G(pA, mA, pP + offset);
    auto sampledOffsetAcceleration = quadrupoleAcceleration.at(offset);

    auto quadrupoleError = glm::distance((glm::vec3) realOffsetAcceleration, sampledOffsetAcceleration);
    auto monopoleError = glm::distance((glm::vec3) realOffsetAcceleration, monopoleAcceleration);

    auto quadrupoleFractionalError = quadrupoleError / glm::length((glm::vec3) realOffsetAcceleration);

    CAPTURE(offset);
    CAPTURE(realOffsetAcceleration);
    CAPTURE(monopoleAcceleration);
    CAPTURE(sampledOffsetAcceleration);

    CHECK(quadrupoleError < monopoleError);
    CHECK(quadrupoleFractionalError < 0.05);
}

TEST_CASE("Re-centering a quadrupole should be reversible", "[Gravity]") {

    Position pA{0, 0, 0};
    Mass mA{2.0};
    Position pP{2, 0, 0};
    Rule G{2.0};

    auto quadrupoleAcceleration = G(pA, mA, pP, NBody::QuadrupoleAccelerationSummary{});

    glm::vec3 offset{
            GENERATE(0.01, 0.1),
            GENERATE(0.01, 0.1),
            GENERATE(0.01, 0.1)
    };

    auto movedQuadrupoleAcceleration = quadrupoleAcceleration.translated(offset);

    // Quadrupole evaluated at an offset should match the base acceleration at that offset
    CHECK(quadrupoleAcceleration.at(offset) == movedQuadrupoleAcceleration.acceleration());
    CHECK(quadrupoleAcceleration.acceleration() == movedQuadrupoleAcceleration.at(-offset));

    auto unmovedQuadrupoleAcceleration = movedQuadrupoleAcceleration.translated(-offset);

    // Moving a quadrupole back to its original location should undo the results
    CHECK(quadrupoleAcceleration.acceleration() == unmovedQuadrupoleAcceleration.acceleration());

}

TEST_CASE("Quadrupole should be a good approximation in a real scenario", "[Gravity]") {

    Rule G{1.0};

    std::vector<std::pair<Position, Mass>> activePositionsAndMasses{
            {{12.237058, 7.9972196, 2.7898774}, {0.0570347}},
            {{15.70352,  12.367721, 3.9934757}, {0.112188}},
            {{12.150897, 13.606152, 3.4104824}, {0.371711}},
            {{16.167948, 7.70833,   6.0922756}, {0.144094}}
    };

    Position passivePosition{0.41169, 1.1282315, 19.398197};
    Position samplePosition{0.76725245, 1.0069804, 17.802502};

    MultipoleAcceleration<2> quadrupoleAcceleration{};
    Acceleration actualAcceleration{};
    for (auto &[position, mass] : activePositionsAndMasses) {
        quadrupoleAcceleration += G(position, mass, samplePosition, NBody::QuadrupoleAccelerationSummary{});
        actualAcceleration += G(position, mass, passivePosition);
    }
    Acceleration approximateAcceleration = quadrupoleAcceleration.at(passivePosition - samplePosition);

    auto quadrupoleError = glm::distance((glm::vec3) actualAcceleration, approximateAcceleration);
    //auto monopoleError = glm::distance((glm::vec3) realOffsetAcceleration, monopoleAcceleration);

    auto quadrupoleFractionalError = quadrupoleError / glm::length((glm::vec3) actualAcceleration);
    CAPTURE(actualAcceleration);
    CAPTURE(quadrupoleAcceleration);
    CAPTURE(approximateAcceleration);
    CHECK(quadrupoleFractionalError < 0.00005);
}
