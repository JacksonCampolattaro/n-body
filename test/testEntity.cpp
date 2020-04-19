//
// Created by jackcamp on 4/18/20.
//

#include <catch2/catch.hpp>

#include <sstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>

#include "../src/Model/Entity.h"

using namespace Model;

TEST_CASE("Converting an empty entity to a string", "[Entity]") {

    auto entity = Entity();

    CHECK("[ entity:   ]" == entity.toString());
}

TEST_CASE("Converting a simple entity to a string", "[Entity]") {

    auto entity = Entity();
    entity.setPosition({1, 2, 3});
    entity.setVelocity({4, 5, 6});

    CHECK("[ entity: (1.000000, 2.000000, 3.000000) <4.000000, 5.000000, 6.000000> ]" == entity.toString());
}
