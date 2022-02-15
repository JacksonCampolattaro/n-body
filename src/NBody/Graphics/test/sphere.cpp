#include <catch2/catch.hpp>

#include <NBody/Graphics/Sphere.h>

#include <iostream>

using Sphere = NBody::Graphics::Sphere;

TEST_CASE("Sphere can be serialized and deserialized", "[Sphere]") {

    Sphere in = {2.0f};

    // Convert the value to a json DOM
    json json_in = in;

    // Convert the json object to a string
    std::string serialized = to_string(json_in);
    std::cout << serialized << std::endl;

    // Convert from the string to another json DOM
    json json_out = json::parse(serialized);

    // Load the value
    Sphere out = json_out.get<Sphere>();

    // The new value should be the same as the one that was serialized.
    REQUIRE(in.radius() == out.radius());
}

