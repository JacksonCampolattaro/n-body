#include <catch2/catch.hpp>

#include <NBody/Graphics/Color.h>

#include <iostream>

using Color = NBody::Graphics::Color;

TEST_CASE("Color can be serialized and deserialized", "[Color]") {

    Color in = {1.0f, 0.5f, 0.0f};

    // Convert the value to a json DOM
    json json_in = in;

    // Convert the json object to a string
    std::string serialized = to_string(json_in);

    // Convert from the string to another json DOM
    json json_out = json::parse(serialized);

    // Load the value
    Color out = json_out.get<Color>();

    // The new value should be the same as the one that was serialized.
    REQUIRE(in == out);
}

