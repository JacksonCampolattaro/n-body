//
// Created by jackcamp on 4/18/20.
//

#include <catch2/catch.hpp>

#include <sstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>

#include "../src/Model/Simulation.h"

using namespace Model;

TEST_CASE("Serialization with no entities", "[Simulation]") {

    auto simulation = Simulation();

    std::stringstream stream;
    {
        auto archive = cereal::XMLOutputArchive(stream);
        archive(simulation);
    }

    std::string expected =
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<cereal>\n"
            "\t<value0>\n"
            "\t\t<entities size=\"dynamic\"/>\n"
            "\t</value0>\n"
            "</cereal>\n"
            "\n";

    CHECK(stream.str() == expected);
}

TEST_CASE("Serialization with a single empty entity", "[Simulation]") {

    auto simulation = Simulation();
    simulation.addEntity(std::make_shared<Entity>(Entity()));

    std::stringstream stream;
    {
        auto archive = cereal::XMLOutputArchive(stream);
        archive(simulation);
    }

    std::string expected =
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<cereal>\n"
            "\t<value0>\n"
            "\t\t<entities size=\"dynamic\">\n"
            "\t\t\t<value0>\n"
            "\t\t\t\t<ptr_wrapper>\n"
            "\t\t\t\t\t<id>2147483649</id>\n"
            "\t\t\t\t\t<data>\n"
            "\t\t\t\t\t\t<position>\n"
            "\t\t\t\t\t\t\t<ptr_wrapper>\n"
            "\t\t\t\t\t\t\t\t<id>0</id>\n"
            "\t\t\t\t\t\t\t</ptr_wrapper>\n"
            "\t\t\t\t\t\t</position>\n"
            "\t\t\t\t\t\t<velocity>\n"
            "\t\t\t\t\t\t\t<ptr_wrapper>\n"
            "\t\t\t\t\t\t\t\t<id>0</id>\n"
            "\t\t\t\t\t\t\t</ptr_wrapper>\n"
            "\t\t\t\t\t\t</velocity>\n"
            "\t\t\t\t\t</data>\n"
            "\t\t\t\t</ptr_wrapper>\n"
            "\t\t\t</value0>\n"
            "\t\t</entities>\n"
            "\t</value0>\n"
            "</cereal>\n"
            "\n";

    CHECK(stream.str() == expected);
}