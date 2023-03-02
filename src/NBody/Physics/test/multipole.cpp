//
// Created by Jackson Campolattaro on 2/28/23.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <NBody/Physics/Multipole.h>

using NBody::Multipole;
using NBody::SymmetricTensor3;

using enum NBody::Dimension;

TEST_CASE("An Octupole should contain symmetric matrices of dimensions 3x3x3, 3x3, and 3", "[Multipole]") {

    Multipole<3> octupole;
    REQUIRE(typeid(octupole.tensor<1>()) == typeid(glm::vec3));
    REQUIRE(typeid(octupole.tensor<2>()) == typeid(SymmetricTensor3<2>));
    REQUIRE(typeid(octupole.tensor<3>()) == typeid(SymmetricTensor3<3>));
}


TEST_CASE("A quadrupole can be constructed as a vector and a 3x3 matrix", "[Multipole]") {

    Multipole<2> constructedInPlace{
            glm::vec3{0},
            std::array<float, 6>{0}
    };

    Multipole<2> constructedFromCopies{
            SymmetricTensor3<1>{{0.0f, 1.0f, 2.0f}},
            SymmetricTensor3<2>{{3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}}
    };
}

TEST_CASE("It should be possible to access members of quadrupole matrices", "[Multipole]") {

    Multipole<2> quadrupole{
            glm::vec3{0.0f, 1.0f, 2.0f},
            SymmetricTensor3<2>{{3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}}
    };

    REQUIRE(quadrupole.tensor<1>() == glm::vec3{0.0f, 1.0f, 2.0f});

    REQUIRE(quadrupole.get<X, X>() == 3.0f);

    REQUIRE(quadrupole.get<X, Y>() == 4.0f);
    REQUIRE(quadrupole.get<Y, X>() == 4.0f);

    REQUIRE(quadrupole.get<X, Z>() == 5.0f);
    REQUIRE(quadrupole.get<Z, X>() == 5.0f);

    // etc....
}


