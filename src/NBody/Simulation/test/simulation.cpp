#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include <NBody/Simulation/Simulation.h>

using Simulation = NBody::Simulation;

TEST_CASE("Equality test for a simple scenario", "[Simulation]") {

    Simulation a, b;
    a.newParticle()
            .setPosition({1, 2, 5})
            .setVelocity({4, 5, 8})
            .setMass({100.0f})
            .setColor({0.25, 0.5, 1.0})
            .setSphere({100.0f});
    b.newParticle()
            .setPosition({1, 2, 5})
            .setVelocity({4, 5, 8})
            .setMass({100.0f})
            .setColor({0.25, 0.5, 1.0})
            .setSphere({100.0f});
    a.newParticle()
            .setPosition({1, 2, 4})
            .setVelocity({4, 5, 7})
            .setMass({10.0f})
            .setColor({0.5, 1.0, 0.25})
            .setSphere({10.0f});
    b.newParticle()
            .setPosition({1, 2, 4})
            .setVelocity({4, 5, 7})
            .setMass({10.0f})
            .setColor({0.5, 1.0, 0.25})
            .setSphere({10.0f});
    a.newParticle()
            .setPosition({1, 2, 3})
            .setVelocity({4, 5, 6})
            .setMass({1.0f})
            .setColor({1.0, 0.5, 0.25})
            .setSphere({1.0f});
    b.newParticle()
            .setPosition({1, 2, 3})
            .setVelocity({4, 5, 6})
            .setMass({1.0f})
            .setColor({1.0, 0.5, 0.25})
            .setSphere({1.0f});

    REQUIRE(a == b);

    b.newParticle()
            .setPosition({1, 2, 3})
            .setVelocity({4, 5, 6})
            .setMass({1.0f})
            .setColor({1.0, 0.5, 0.25})
            .setSphere({1.0f});

    REQUIRE(a != b);

}

TEST_CASE("Serialization of a small scenario to JSON", "[Simulation]") {

    // Create a simulation and add a couple of particles
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
    json json_in;
    to_json(json_in, original);

    // Serialize the DOM
    std::string serialized = json_in.dump(4);

    // Parse the string back into a DOM
    json json_out = json::parse(serialized);

    // Extract the simulation
    Simulation deserialized;
    from_json(json_out, deserialized);

    // The fastest way to check for equality is to re-serialize and compare strings
    // The serialization process reverses the order of the particles, so we need to do this twice
    REQUIRE(original == deserialized);
}


TEST_CASE("Copy & move construction", "[Simulation]") {

    // Create a simulation and add a couple of particles
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

    Simulation copyConstructed{original};
    REQUIRE(copyConstructed == original);

    Simulation copyAssigned = original;
    REQUIRE(copyAssigned == original);

}
