//
// Created by Jackson Campolattaro on 2/27/23.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include <NBody/Physics/SymmetricMatrix3.h>

using
enum NBody::Dimension;

TEST_CASE("Member access to a 3x3 symmetric matrix should be correct", "[SymmetricMatrix3]") {

    // Construct a quadrupole with known values
    NBody::SymmetricMatrix3<2> q{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f}
    };

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

TEST_CASE("Member access to a 3x3x3 symmetric matrix should be correct", "[SymmetricMatrix3]") {

    // Construct a quadrupole with known values
    NBody::SymmetricMatrix3<3> q{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f}
    };

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

    REQUIRE(q.get<X, Y, X>() == 1.0f);
    REQUIRE(q.get<Z, Y, X>() == 4.0f);
}

TEST_CASE("Equality and inequality of symmetric matrices", "[SymmetricMatrix3]") {

    NBody::SymmetricMatrix3<1> s1a{0.0f, 1.0f, 2.0f};
    NBody::SymmetricMatrix3<1> s1b{};
    REQUIRE(s1a == s1a);
    REQUIRE(s1a != s1b);

    NBody::SymmetricMatrix3<2> s2a{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f}
    };
    NBody::SymmetricMatrix3<2> s2b{
            {0}
    };
    REQUIRE(s2a == s2a);
    REQUIRE(s2a != s2b);

    NBody::SymmetricMatrix3<3> s3a{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f}
    };
    NBody::SymmetricMatrix3<3> s3b{
            {0}
    };
    REQUIRE(s3a == s3a);
    REQUIRE(s3a != s3b);

}

TEST_CASE("Addition and subtraction of symmetric matrices", "[SymmetricMatrix3]") {

    NBody::SymmetricMatrix3<3> s3a{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f}
    };
    NBody::SymmetricMatrix3<3> s3b = s3a + s3a;
    NBody::SymmetricMatrix3<3> s3c = s3a + s3b;

    REQUIRE(s3b.get<X, X, Y>() == 2.0f);
    REQUIRE(s3c.get<X, X, Y>() == 3.0f);

    s3c -= s3b;
    REQUIRE(s3c == s3a);

    s3a += s3a;
    REQUIRE(s3a == s3b);
}

TEST_CASE("Linear indices can be converted to their dimensional counterparts in standard form", "[SymmetricMatrix3]") {

    //std::array<NBody::Dimension, 3> index3;

    constexpr auto linearIndexA = NBody::SymmetricMatrix3<3>::linearIndex<X, Y, Z>();
    constexpr auto indexA = NBody::SymmetricMatrix3<3>::dimensionalIndex<linearIndexA>();
    CHECK(indexA == std::array<NBody::Dimension, 3>{X, Y, Z});

    constexpr auto linearIndexB = NBody::SymmetricMatrix3<3>::linearIndex<Z, Y, Z>();
    constexpr auto indexB = NBody::SymmetricMatrix3<3>::dimensionalIndex<linearIndexB>();
    CHECK(indexB == std::array<NBody::Dimension, 3>{Y, Z, Z});

    constexpr auto linearIndexC = NBody::SymmetricMatrix3<5>::linearIndex<Z, Y, Z, Y, X>();
    constexpr auto indexC = NBody::SymmetricMatrix3<5>::dimensionalIndex<linearIndexC>();
    CHECK(indexC == std::array<NBody::Dimension, 5>{X, Y, Y, Z, Z});
}

TEST_CASE("The outer product of two SymmetricMatrix3<1>s is a SymmetricMatrix3<2>", "[SymmetricMatrix3]") {

    // todo
}
