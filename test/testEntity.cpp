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

    CHECK("[ entity: (0.000000, 0.000000, 0.000000) <0.000000, 0.000000, 0.000000> ]" == entity.toString());
}

TEST_CASE("Converting a simple entity to a string", "[Entity]") {

    auto entity = Entity();
    entity.setPosition({1, 2, 3});
    entity.setVelocity({4, 5, 6});

    CHECK("[ entity: (1.000000, 2.000000, 3.000000) <4.000000, 5.000000, 6.000000> ]" == entity.toString());
}

TEST_CASE("Serializing the default Entity", "[Entity]") {

    auto entity = Entity();

    std::stringstream stream;
    {
        auto archive = cereal::XMLOutputArchive(stream);
        archive(entity);
    }

    std::string result = stream.str();

    std::string expected =
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<cereal>\n"
            "\t<value0>\n"
            "\t\t<position>\n"
            "\t\t\t<x>0</x>\n"
            "\t\t\t<y>0</y>\n"
            "\t\t\t<z>0</z>\n"
            "\t\t</position>\n"
            "\t\t<velocity>\n"
            "\t\t\t<x>0</x>\n"
            "\t\t\t<y>0</y>\n"
            "\t\t\t<z>0</z>\n"
            "\t\t</velocity>\n"
            "\t</value0>\n"
            "</cereal>\n"
            "\n";

    CHECK(result == expected);
}

TEST_CASE("Serializing a simple Entity", "[Entity]") {

    auto entity = Entity();
    entity.setPosition({1, 2, 3});
    entity.setVelocity({4, 5, 6});

    std::stringstream stream;
    {
        auto archive = cereal::XMLOutputArchive(stream);
        archive(entity);
    }

    std::string result = stream.str();

    std::string expected =
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<cereal>\n"
            "\t<value0>\n"
            "\t\t<position>\n"
            "\t\t\t<x>1</x>\n"
            "\t\t\t<y>2</y>\n"
            "\t\t\t<z>3</z>\n"
            "\t\t</position>\n"
            "\t\t<velocity>\n"
            "\t\t\t<x>4</x>\n"
            "\t\t\t<y>5</y>\n"
            "\t\t\t<z>6</z>\n"
            "\t\t</velocity>\n"
            "\t</value0>\n"
            "</cereal>\n"
            "\n";

    CHECK(result == expected);
}
