#include <catch2/catch.hpp>

#include <NBody/Physics/Mass.h>

#include <iostream>

using ActiveMass = NBody::Physics::Mass;

TEST_CASE("Mass can be serialized and deserialized", "[Mass]") {

    ActiveMass in = {2.0f};

    // Convert the value to a json DOM
    json json_in = in;

    // Convert the json object to a string
    std::string serialized = to_string(json_in);

    // Convert from the string to another json DOM
    json json_out = json::parse(serialized);

    // Load the value
    ActiveMass out = json_out.get<ActiveMass>();

    // The new value should be the same as the one that was serialized.
    REQUIRE(in == out);
}