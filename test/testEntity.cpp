#include <cassert>

#include "../src/Model/Simulation.h"
#include "../src/Model/Entity.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

using Model::Simulation;
using Model::Entity;

int main() {

    rapidjson::StringBuffer buffer;

    Simulation simulation{};

    auto original = simulation.newEntity();
    original.setPosition({1, 2, 3})
            .setVelocity({4, 5, 6})
            .setDrawable({{0.1, 0.2, 0.3}, 100})
            .setPassiveElement({200})
            .setActiveElement({300});
    std::cout << original << std::endl;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writer << original;

    std::string saved = buffer.GetString();
    std::cout << "archive: \n" << saved << std::endl;

    simulation = Simulation();
    auto copy = simulation.newEntity();
    rapidjson::Document document;
    document.Parse(saved.c_str());
    document >> copy;
    std::cout << "copy: " << copy << std::endl;

}
