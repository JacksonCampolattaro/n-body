#include <catch2/catch.hpp>

#include <NBody/Physics/Gravity.h>

#include <iostream>

using NBody::Physics::Position;
using NBody::Physics::ActiveMass;
using NBody::Physics::PassiveMass;
using NBody::Physics::Rule;
using NBody::Physics::Gravity;
using NBody::Physics::Force;

TEST_CASE("Gravity rule computes the force between two particles", "[Gravity]") {

    Position pA = {0, 0, 0};
    ActiveMass mA = {2.0};

    Position pB = {2, 0, 0};
    PassiveMass mB = {1.0};

    Gravity G = {2.0};

    auto r = G(pA, mA, pB, mB);
    Position p = r.first;
    Force f = r.second;

    REQUIRE(f == Force{{1, 0, 0}});
    REQUIRE(p == Position{1, 0, 0});

}
