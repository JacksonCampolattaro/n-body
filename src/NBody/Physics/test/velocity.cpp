#include <catch2/catch.hpp>

#include <NBody/Physics/Velocity.h>

#include <iostream>

using Velocity = NBody::Physics::Velocity;

TEST_CASE("Velocity can be serialized and deserialized", "[Velocity]") {

    Velocity in = {1.0f, 2.0f, 3.0f};

    // Convert the position to a json DOM
    json json_in = in;

    // Convert the json object to a string
    std::string serialized = to_string(json_in);

    // Convert from the string to another json DOM
    json json_out = json::parse(serialized);

    // Load the position
    Velocity out = json_out.get<Velocity>();

    // The new position should be the same as the one that was serialized.
    REQUIRE(in == out);
}