#include <catch2/catch.hpp>

#include <NBody/Simulation/Simulation.h>

using namespace NBody::Simulation;

TEST_CASE("Serialization", "[Simulation]") {

    // Create a simple simulation scenario

    Simulation original;

    Entity a = original.createBody();
    original.emplace_or_replace<Position>(a, 0.0f, 5.0f, 5.0f);
    original.emplace_or_replace<Velocity>(a, 0.0f, -0.1f, 5.0f);

    Entity b = original.createBody();
    original.emplace_or_replace<Position>(b, 0.0f, 5.0f, 4.0f);
    original.emplace_or_replace<Velocity>(b, 0.0f, -0.2f, 5.0f);

    Entity c = original.createBody();
    original.emplace_or_replace<Position>(c, 0.0f, 5.0f, 3.0f);
    original.emplace_or_replace<Velocity>(c, 1.0f, -0.3f, 5.0f);

    // Serialize it
    std::stringstream stream;
    stream << original;
    std::cout << stream.str();

    // Load it again
    Simulation deserialized;
    stream >> deserialized;

    // Make sure the new one is identical
    // (The easiest way of doing that is to compare their serialized data)
    std::stringstream stream_copy, stream_original;
    stream_original << original;
    stream_copy << deserialized;
    CHECK(stream_copy.str() == stream_original.str());
}
