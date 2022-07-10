#include <catch2/catch.hpp>

#include <NBody/Simulation/Simulation.h>

using Simulation = NBody::Simulation;


TEST_CASE("Serialization of a single particle", "[Simulation]") {

    // Create a simulation and add one particle
    Simulation original;
    original.newParticle()
            .setPosition({1, 2, 3})
            .setVelocity({4, 5, 6})
            .setMass({100.0f})
            .setColor({0.25, 0.5, 1.0})
            .setSphere({10.0f});

    // Create a json DOM of the simulation
    json json_in = original;

    // Write the DOM to a string
    std::string serialized = to_string(json_in);

    // Deserialize into another json
    json json_out = json::parse(serialized);

    // Extract the simulation
    Simulation deserialized = json_out.get<Simulation>();

    // The fastest way to check for equality is to re-serialize and compare strings
    json j = deserialized;
    std::string re_serialized = to_string(j);
    REQUIRE(serialized == re_serialized);
}

TEST_CASE("Serialization of a small scenario", "[Simulation]") {

    // Create a simulation and add one particle
    Simulation original;
    original.newParticle()
            .setPosition({1, 2, 5})
            .setVelocity({4, 5, 8})
            .setMass({100.0f})
            .setColor({0.25, 0.5, 1.0})
            .setSphere({100.0f});
    original.newParticle()
            .setPosition({1, 2, 4})
            .setVelocity({4, 5, 7})
            .setMass({10.0f})
            .setColor({0.5, 1.0, 0.25})
            .setSphere({10.0f});
    original.newParticle()
            .setPosition({1, 2, 3})
            .setVelocity({4, 5, 6})
            .setMass({1.0f})
            .setColor({1.0, 0.5, 0.25})
            .setSphere({1.0f});

    // Create a json DOM of the simulation
    json json_in = original;

    // Serialize the DOM
    std::string serialized = json_in.dump(4);

    // Parse the string back into a DOM
    json json_out = json::parse(serialized);

    // Extract the simulation
    Simulation deserialized = json_out.get<Simulation>();

    // The fastest way to check for equality is to re-serialize and compare strings
    // The serialization process reverses the order of the particles, so we need to do this twice
    json a = deserialized;
    std::string re_serialized_backwards = a.dump(4);
    Simulation reversed = json::parse(re_serialized_backwards).get<Simulation>();
    json b = reversed;
    std::string re_serialized = b.dump(4);
    REQUIRE(serialized == re_serialized);
}

