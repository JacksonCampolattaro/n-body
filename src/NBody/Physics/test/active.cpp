#include <catch2/catch.hpp>

#include <NBody/Physics/ActiveMass.h>

#include <iostream>

using ActiveMass = NBody::Physics::ActiveMass;

TEST_CASE("ActiveMass can be serialized and deserialized", "[ActiveMass]") {

    ActiveMass in = {2.0f};

    // Convert the position to a json DOM
    json json_in = in;

    // Convert the json object to a string
    std::string serialized = to_string(json_in);

    // Convert from the string to another json DOM
    json json_out = json::parse(serialized);

    // Load the position
    ActiveMass out = json_out.get<ActiveMass>();

    // The new position should be the same as the one that was serialized.
    REQUIRE(in == out);
}