#include <cassert>

#include "../src/Model/Simulation.h"
#include "../src/Model/Entity.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

#include <iostream>

using Model::Simulation;
using Model::Entity;

int main() {

    rapidjson::StringBuffer buffer;

    Simulation original{};

    original.newEntity()
            .setPosition({0, 5, 5})
            .setVelocity({0, -0.1, 0})
            .setDrawable({{0.8, 0.8, 0.8}, 1.0})
            .setPassiveElement({0.5})
            .setActiveElement({0.5});
    original.newEntity()
            .setPosition({-5, 0, 0})
            .setVelocity({0, 0.15, 0})
            .setDrawable({{0.8, 0.8, 0.0}, 1.0})
            .setPassiveElement({0.5})
            .setActiveElement({0.5});
    original.newEntity()
            .setPosition({5, 0, 0})
            .setVelocity({0, -0.05, 0})
            .setDrawable({{0.8, 0.0, 0.0}, 1.0})
            .setPassiveElement({0.5})
            .setActiveElement({0.5});
    std::cout << "original: \n" << original;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writer << original;
    original.saveBodiesToPath("test.bod");

    std::string saved = buffer.GetString();
    std::cout << "archive: \n" << saved << std::endl;


    auto copy = Simulation();
    rapidjson::Document document;
    document.Parse(saved.c_str());
    document >> copy;
    std::cout << "copy: " << copy << std::endl;

}
