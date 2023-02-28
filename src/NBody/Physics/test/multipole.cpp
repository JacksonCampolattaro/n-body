//
// Created by Jackson Campolattaro on 2/27/23.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include <NBody/Physics/SymmetricMatrix3.h>

TEST_CASE("Member access to a Quadrupole should be correct", "[SymmetricMatrix3]") {

    // Construct a quadrupole with known values
    NBody::SymmetricMatrix3<2> q{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f}
    };

    const std::size_t X = 0;
    const std::size_t Y = 1;
    const std::size_t Z = 2;

    REQUIRE(q.get<X, X>() == 0.0f);

    REQUIRE(q.get<X, Y>() == 1.0f);
    REQUIRE(q.get<Y, X>() == 1.0f);

    REQUIRE(q.get<X, Z>() == 2.0f);
    REQUIRE(q.get<Z, X>() == 2.0f);

    REQUIRE(q.get<Y, Y>() == 3.0f);

    REQUIRE(q.get<Y, Z>() == 4.0f);
    REQUIRE(q.get<Z, Y>() == 4.0f);

    REQUIRE(q.get<Z, Z>() == 5.0f);
}

TEST_CASE("Member access to an Octupole should be correct", "[SymmetricMatrix3]") {

    // Construct a quadrupole with known values
    NBody::SymmetricMatrix3<3> q{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f}
    };

    const std::size_t X = 0;
    const std::size_t Y = 1;
    const std::size_t Z = 2;

    REQUIRE(q.get<X, X, X>() == 0.0f);
    REQUIRE(q.get<X, X, Y>() == 1.0f);
    REQUIRE(q.get<X, X, Z>() == 2.0f);
    REQUIRE(q.get<X, Y, Y>() == 3.0f);
    REQUIRE(q.get<X, Y, Z>() == 4.0f);
    REQUIRE(q.get<X, Z, Z>() == 5.0f);
    REQUIRE(q.get<Y, Y, Y>() == 6.0f);
    REQUIRE(q.get<Y, Y, Z>() == 7.0f);
    REQUIRE(q.get<Y, Z, Z>() == 8.0f);
    REQUIRE(q.get<Z, Z, Z>() == 9.0f);
}

TEST_CASE("The outer product of two SymmetricMatrix3<1>s is a SymmetricMatrix3<2>", "[SymmetricMatrix3]") {

    // todo
}
