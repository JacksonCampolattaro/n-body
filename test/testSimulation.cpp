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
/*

TEST_CASE("Serialization with a single default entity", "[Simulation]") {

    auto simulation = Simulation();
    auto entity = Entity();
    simulation.addEntity(entity);

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
            "\t\t\t\t<position>\n"
            "\t\t\t\t\t<x>0</x>\n"
            "\t\t\t\t\t<y>0</y>\n"
            "\t\t\t\t\t<z>0</z>\n"
            "\t\t\t\t</position>\n"
            "\t\t\t\t<velocity>\n"
            "\t\t\t\t\t<x>0</x>\n"
            "\t\t\t\t\t<y>0</y>\n"
            "\t\t\t\t\t<z>0</z>\n"
            "\t\t\t\t</velocity>\n"
            "\t\t\t</value0>\n"
            "\t\t</entities>\n"
            "\t</value0>\n"
            "</cereal>\n"
            "\n";

    std::string result = stream.str();

    CHECK(result == expected);
}

TEST_CASE("Serialization with a single simple entity", "[Simulation]") {

    auto simulation = Simulation();
    auto entity = Entity();
    entity.setPosition({1, 2, 3});
    entity.setVelocity({4, 5, 6});
    simulation.addEntity(entity);

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
            "\t\t\t\t\t\t\t\t<id>2147483650</id>\n"
            "\t\t\t\t\t\t\t\t<data>\n"
            "\t\t\t\t\t\t\t\t\t<x>1</x>\n"
            "\t\t\t\t\t\t\t\t\t<y>2</y>\n"
            "\t\t\t\t\t\t\t\t\t<z>3</z>\n"
            "\t\t\t\t\t\t\t\t</data>\n"
            "\t\t\t\t\t\t\t</ptr_wrapper>\n"
            "\t\t\t\t\t\t</position>\n"
            "\t\t\t\t\t\t<velocity>\n"
            "\t\t\t\t\t\t\t<ptr_wrapper>\n"
            "\t\t\t\t\t\t\t\t<id>2147483651</id>\n"
            "\t\t\t\t\t\t\t\t<data>\n"
            "\t\t\t\t\t\t\t\t\t<Δx>4</Δx>\n"
            "\t\t\t\t\t\t\t\t\t<Δy>5</Δy>\n"
            "\t\t\t\t\t\t\t\t\t<Δz>6</Δz>\n"
            "\t\t\t\t\t\t\t\t</data>\n"
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
*/
