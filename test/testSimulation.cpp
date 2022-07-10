#include <catch2/catch.hpp>

#include <NBody/Simulation/Simulation.h>

using namespace NBody::Simulation;
using namespace NBody::Physics;
using namespace NBody::Graphics;

TEST_CASE("Serialization of a single body", "[Simulation]") {

    // Create a simple simulation scenario
    Simulation original;

    Entity a = original.createBody();
    original.emplace_or_replace<Position>(a, 0.0f, 5.0f, 5.0f);
    original.emplace_or_replace<Velocity>(a, 0.0f, -0.1f, 5.0f);

    // Serialize the scenario
    std::stringstream stream;
    stream << original;

    // Deserialize that data
    Simulation copy;
    stream >> copy;

    std::stringstream original_stream, copy_stream;
    original_stream << original;
    copy_stream << copy;
    CHECK(original_stream.str() == copy_stream.str());
}

TEST_CASE("Serialization of a small scenario", "[Simulation]") {

    // Create a simple simulation scenario
    Simulation original;

    Entity a = original.createBody();
    original.emplace_or_replace<Position>(a, 0.0f, 5.0f, 5.0f);
    original.emplace_or_replace<Velocity>(a, 0.0f, -0.1f, 5.0f);

    Entity b = original.createBody();
    original.emplace_or_replace<Position>(b, 0.0f, 5.0f, 4.0f);
    original.emplace_or_replace<Velocity>(b, 0.0f, -0.2f, 5.0f);
    original.emplace_or_replace<Color>(b, Magnum::Color3::red());
    original.emplace_or_replace<Sphere>(b, 4.0f);

    Entity c = original.createBody();
    original.emplace_or_replace<Position>(c, 0.0f, 5.0f, 3.0f);
    original.emplace_or_replace<Velocity>(c, 1.0f, -0.3f, 5.0f);
    original.emplace_or_replace<ActiveMass>(c, 5.0f);

    // Serialize the scenario
    std::stringstream stream;
    stream << original;

    std::cout << stream.str() << std::endl;

    // Deserialize that data
    Simulation intermediate;
    stream >> intermediate;

    // Data gets reversed when serialized, so we need to repeat to put it in the same order for testing
    stream << intermediate;
    Simulation copy;
    stream >> copy;

    std::stringstream original_stream, copy_stream;
    original_stream << original;
    copy_stream << copy;
    CHECK(original_stream.str() == copy_stream.str());
}
