//
// Created by Jackson Campolattaro on 2/27/23.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <NBody/Physics/SymmetricTensor3.h>
#include <glm/matrix.hpp>

#include <iostream>
#include <numeric>

using namespace NBody;
using
enum Dimension;

TEST_CASE("Member access to a 3x3 symmetric tensor should be correct", "[SymmetricTensor3]") {

    // Construct a quadrupole with known values
    NBody::SymmetricTensor3<2> q{
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

TEST_CASE("Member access to a 3x3x3 symmetric tensor should be correct", "[SymmetricTensor3]") {

    // Construct a quadrupole with known values
    NBody::SymmetricTensor3<3> q{
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

TEST_CASE("Equality and inequality of symmetric tensors", "[SymmetricTensor3]") {

    //    NBody::SymmetricTensor3<1> s1a{0.0f, 1.0f, 2.0f};
    //    NBody::SymmetricTensor3<1> s1b{};
    //    REQUIRE(s1a == s1a);
    //    REQUIRE(s1a != s1b);

    NBody::SymmetricTensor3<2> s2a{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f}
    };
    NBody::SymmetricTensor3<2> s2b{
            {0}
    };
    REQUIRE(s2a == s2a);
    REQUIRE(s2a != s2b);

    NBody::SymmetricTensor3<3> s3a{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f}
    };
    NBody::SymmetricTensor3<3> s3b{
            {0}
    };
    REQUIRE(s3a == s3a);
    REQUIRE(s3a != s3b);

}

TEST_CASE("Addition and subtraction of symmetric tensors", "[SymmetricTensor3]") {

    NBody::SymmetricTensor3<3> s3a{
            {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f}
    };
    NBody::SymmetricTensor3<3> s3b = s3a + s3a;
    NBody::SymmetricTensor3<3> s3c = s3a + s3b;

    REQUIRE(s3b.get<X, X, Y>() == 2.0f);
    REQUIRE(s3c.get<X, X, Y>() == 3.0f);

    s3c -= s3b;
    REQUIRE(s3c == s3a);

    s3a += s3a;
    REQUIRE(s3a == s3b);

    std::vector<SymmetricTensor3<2>> identities{
            SymmetricTensor3<2>::identity(),
            SymmetricTensor3<2>::identity(),
            SymmetricTensor3<2>::identity(),
            SymmetricTensor3<2>::identity(),
            SymmetricTensor3<2>::identity()
    };
    auto identity5 = std::reduce(identities.begin(), identities.end());
    REQUIRE(identity5 == SymmetricTensor3<2>::identity() * 5);
}

TEST_CASE("Linear indices can be converted to their dimensional counterparts in standard form", "[SymmetricTensor3]") {

    //std::array<NBody::Dimension, 3> index3;

    constexpr auto linearIndexA = NBody::SymmetricTensor3<3>::linearIndex<X, Y, Z>();
    constexpr auto indexA = NBody::SymmetricTensor3<3>::dimensionalIndex<linearIndexA>();
    CHECK(indexA == std::array<NBody::Dimension, 3>{X, Y, Z});

    constexpr auto linearIndexB = NBody::SymmetricTensor3<3>::linearIndex<Z, Y, Z>();
    constexpr auto indexB = NBody::SymmetricTensor3<3>::dimensionalIndex<linearIndexB>();
    CHECK(indexB == std::array<NBody::Dimension, 3>{Y, Z, Z});

    constexpr auto linearIndexC = NBody::SymmetricTensor3<5>::linearIndex<Z, Y, Z, Y, X>();
    constexpr auto indexC = NBody::SymmetricTensor3<5>::dimensionalIndex<linearIndexC>();
    CHECK(indexC == std::array<NBody::Dimension, 5>{X, Y, Y, Z, Z});
}

TEST_CASE("A simple nullary function can initialize a symmetric tensor", "[SymmetricTensor3]") {

    auto ones = NBody::SymmetricTensor3<3>::nullary([](std::array<Dimension, 3> _) {
        return 1.0f;
    });
    CAPTURE(ones.flat());
    REQUIRE(ones ==
            NBody::SymmetricTensor3<3>{{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}});

    int i = 0;
    auto sequence = NBody::SymmetricTensor3<3>::nullary([&](std::array<Dimension, 3> _) {
        return i++;
    });
    CAPTURE(sequence.flat());
    REQUIRE(sequence ==
            NBody::SymmetricTensor3<3>{{0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f}});

    auto sumOfDimensions = NBody::SymmetricTensor3<3>::nullary([&](std::array<Dimension, 3> dimensions) {
        return (float) dimensions[0] + (float) dimensions[1] + (float) dimensions[2];
    });
    CAPTURE(sumOfDimensions.flat());
    REQUIRE(sumOfDimensions ==
            NBody::SymmetricTensor3<3>{{0.0f, 1.0f, 2.0f, 2.0f, 3.0f, 4.0f, 3.0f, 4.0f, 5.0f, 6.0f}});

}

TEST_CASE("The cartesian power of a vector (repeated outer product) can produce a tensor", "[SymmetricTensor3]") {

    auto power2 = NBody::SymmetricTensor3<2>::cartesianPower({1.0f, 2.0f, 3.0f});
    CAPTURE(power2.flat());
    REQUIRE(power2 ==
            NBody::SymmetricTensor3<2>{{1.0f, 2.0f, 3.0f, 4.0f, 6.0f, 9.0f}});

    auto power3 = NBody::SymmetricTensor3<3>::cartesianPower({1.0f, 2.0f, 3.0f});
    CAPTURE(power3.flat());
    REQUIRE(power3 ==
            NBody::SymmetricTensor3<3>{{1.0f, 2.0f, 3.0f, 4.0f, 6.0f, 9.0f, 8.0f, 12.0f, 18.0f, 27.0f}});

}

TEST_CASE("The trace of a tensor can be found and eliminated", "[SymmetricTensor3]") {

    auto power2 = NBody::SymmetricTensor3<2>::cartesianPower({1.0f, 2.0f, 3.0f});
    CAPTURE(power2.flat());
    REQUIRE(power2.trace() == 14.0f);
    auto traceless2 = power2.traceless();
    CAPTURE(traceless2.flat());
    REQUIRE(traceless2.get<X, X>() == Catch::Approx(-3.66667f));
    REQUIRE(traceless2.get<Y, Y>() == Catch::Approx(-0.66667f));
    REQUIRE(traceless2.get<Z, Z>() == Catch::Approx(4.33333f));

    auto power3 = NBody::SymmetricTensor3<3>::cartesianPower({1.0f, 2.0f, 3.0f});
    CAPTURE(power3.flat());
    REQUIRE(power3.trace() == 36.0f);
    auto traceless3 = power3.traceless();
    CAPTURE(traceless3.flat());
    REQUIRE(traceless3 ==
            NBody::SymmetricTensor3<3>{{1.0f, -10.0f, -9.0f, 4.0f, -6.0f, 9.0f, -4.0f, 12.0f, 6.0f, 27.0f}});

}

TEST_CASE("The sum of a tensors elements can be computed", "[SymmetricTensor3]") {

    auto power2 = NBody::SymmetricTensor3<2>::cartesianPower({1.0f, 2.0f, 3.0f});
    CAPTURE(power2.flat());
    REQUIRE(power2.sum() == 36.0f);

    auto power3 = NBody::SymmetricTensor3<3>::cartesianPower({1.0f, 2.0f, 3.0f});
    CAPTURE(power3.flat());
    REQUIRE(power3.sum() == 216.0f);

}

TEST_CASE("Matrix-vector multiplication should produce correct results", "[SymmetricTensor3]") {

    auto identityProduct = NBody::SymmetricTensor3<2>::identity() * glm::vec3{1.0f, 2.0f, 3.0f};
    CAPTURE(identityProduct.x);
    CAPTURE(identityProduct.y);
    CAPTURE(identityProduct.z);
    //REQUIRE(identityProduct == glm::vec3{1.0f, 2.0f, 3.0f});

    auto ones = NBody::SymmetricTensor3<2>::nullary([](auto _) { return 1.0f; });
    auto glmOnes = glm::outerProduct(glm::vec3{1, 1, 1}, glm::vec3{1, 1, 1});
    auto productWithOnes = ones * glm::vec3{1.0f, 2.0f, 3.0f};
    auto productWithGLMOnes = glmOnes * glm::vec3{1.0f, 2.0f, 3.0f};
    CAPTURE(productWithOnes.x);
    CAPTURE(productWithOnes.y);
    CAPTURE(productWithOnes.z);
    CAPTURE(productWithGLMOnes.x);
    CAPTURE(productWithGLMOnes.y);
    CAPTURE(productWithGLMOnes.z);
    REQUIRE(productWithOnes == productWithGLMOnes);

    // More complex case, with some arbitrary vectors and tensors.
    // Checked with glm
    glm::vec3 v{1.0f, 2.0f, 3.0f};
    auto power2 = NBody::SymmetricTensor3<2>::cartesianPower(v);
    auto glmPower2 = glm::outerProduct(v, v);
    auto power2Copy = NBody::SymmetricTensor3<2>::nullary([&](auto dimensions) {
        return glmPower2[(std::size_t) dimensions[0]][(std::size_t) dimensions[1]];
    });
    glm::vec3 v2{1.0f, 0.0f, 0.0f};
    REQUIRE(power2 == power2Copy);
    auto productWithPower2 = power2 * v2;
    CAPTURE(productWithPower2.x);
    CAPTURE(productWithPower2.y);
    CAPTURE(productWithPower2.z);
    auto productWithGLMPower2 = glmPower2 * v2;
    CAPTURE(productWithGLMPower2.x);
    CAPTURE(productWithGLMPower2.y);
    CAPTURE(productWithGLMPower2.z);
    REQUIRE(productWithPower2 == productWithGLMPower2);

}

TEST_CASE("Lexicographical Indices", "[SymmetricTensor3]") {

    // 2d tensor indexing
    REQUIRE(SymmetricTensor3<2>::lexicographicalIndex<0>() == std::array<Dimension, 2>{X, X});
    REQUIRE(SymmetricTensor3<2>::lexicographicalIndex<1>() == std::array<Dimension, 2>{X, Y});
    REQUIRE(SymmetricTensor3<2>::lexicographicalIndex<2>() == std::array<Dimension, 2>{X, Z});
    REQUIRE(SymmetricTensor3<2>::lexicographicalIndex<3>() == std::array<Dimension, 2>{Y, X});
    REQUIRE(SymmetricTensor3<2>::lexicographicalIndex<4>() == std::array<Dimension, 2>{Y, Y});
    REQUIRE(SymmetricTensor3<2>::lexicographicalIndex<5>() == std::array<Dimension, 2>{Y, Z});
    REQUIRE(SymmetricTensor3<2>::lexicographicalIndex<6>() == std::array<Dimension, 2>{Z, X});
    REQUIRE(SymmetricTensor3<2>::lexicographicalIndex<7>() == std::array<Dimension, 2>{Z, Y});
    REQUIRE(SymmetricTensor3<2>::lexicographicalIndex<8>() == std::array<Dimension, 2>{Z, Z});

    // 3d tensor indexing
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<0>() == std::array<Dimension, 3>{X, X, X});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<1>() == std::array<Dimension, 3>{X, X, Y});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<2>() == std::array<Dimension, 3>{X, X, Z});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<3>() == std::array<Dimension, 3>{X, Y, X});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<4>() == std::array<Dimension, 3>{X, Y, Y});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<5>() == std::array<Dimension, 3>{X, Y, Z});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<6>() == std::array<Dimension, 3>{X, Z, X});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<7>() == std::array<Dimension, 3>{X, Z, Y});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<8>() == std::array<Dimension, 3>{X, Z, Z});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<9>() == std::array<Dimension, 3>{Y, X, X});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<10>() == std::array<Dimension, 3>{Y, X, Y});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<11>() == std::array<Dimension, 3>{Y, X, Z});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<12>() == std::array<Dimension, 3>{Y, Y, X});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<13>() == std::array<Dimension, 3>{Y, Y, Y});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<14>() == std::array<Dimension, 3>{Y, Y, Z});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<15>() == std::array<Dimension, 3>{Y, Z, X});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<16>() == std::array<Dimension, 3>{Y, Z, Y});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<17>() == std::array<Dimension, 3>{Y, Z, Z});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<18>() == std::array<Dimension, 3>{Z, X, X});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<19>() == std::array<Dimension, 3>{Z, X, Y});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<20>() == std::array<Dimension, 3>{Z, X, Z});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<21>() == std::array<Dimension, 3>{Z, Y, X});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<22>() == std::array<Dimension, 3>{Z, Y, Y});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<23>() == std::array<Dimension, 3>{Z, Y, Z});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<24>() == std::array<Dimension, 3>{Z, Z, X});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<25>() == std::array<Dimension, 3>{Z, Z, Y});
    REQUIRE(SymmetricTensor3<3>::lexicographicalIndex<26>() == std::array<Dimension, 3>{Z, Z, Z});
}

TEST_CASE("Matrix-Matrix outer product (3x3 * 3x3 --> 3x3x3)", "[SymmetricTensor3]") {

    glm::vec3 a{1.0f, 2.0f, 3.0f};

    // todo

}
