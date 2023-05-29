#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include <glm/gtx/string_cast.hpp>

#include "NBody/Physics/Rules/Gravity.h"

using namespace NBody::Physics;

TEST_CASE("Intersection tests", "[BoundingBox]") {

    BoundingBox box{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};

    BoundingBox overlapping1{{0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 1.5f}};
    BoundingBox overlapping2{{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}};
    BoundingBox overlapping3{{0.25f, 0.25f, -0.5f}, {0.75f, 0.75f, 1.5f}};
    BoundingBox fullyContained{{0.25f, 0.25f, 0.25f}, {0.75f, 0.75f, 0.75f}};

    CHECK(doIntersect(box, overlapping1));
    CHECK(doIntersect(box, overlapping2));
    CHECK(doIntersect(box, overlapping3));
    CHECK(doIntersect(box, fullyContained));
    CHECK(doIntersect(fullyContained, box));

    BoundingBox nonOverlapping1{{0.0f, 0.0f, 10.0f}, {1.0f, 1.0f, 11.0f}};
    BoundingBox nonOverlapping2{{10.0f, 10.0f, 0.0f}, {11.0f, 11.0f, 1.0f}};

    CHECK(!doIntersect(box, nonOverlapping1));
    CHECK(!doIntersect(box, nonOverlapping2));
}

