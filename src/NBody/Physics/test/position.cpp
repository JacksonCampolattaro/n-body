#include <catch2/catch.hpp>

#include <NBody/Physics/Position.h>

#include <iostream>

using Position = NBody::Physics::Position;

TEST_CASE("Position can be serialized and deserialized", "[Position]") {

    Position in = {1.0f, 2.0f, 3.0f};

    // Convert the value to a json DOM
    json json_in = in;

    // Convert the json object to a string
    std::string serialized = to_string(json_in);

    // Convert from the string to another json DOM
    json json_out = json::parse(serialized);

    // Load the value
    Position out = json_out.get<Position>();

    // The new value should be the same as the one that was serialized.
    REQUIRE(in == out);
}