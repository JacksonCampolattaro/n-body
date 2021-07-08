
#include <NBody/Simulation/Simulation.h>

using namespace NBody;

void cubicGrid(Simulation::Simulation &simulation,
               Physics::Position corner, glm::ivec3 size, float spacing,
               Physics::Velocity velocity, float mass) {

    glm::vec3 fsize = size;

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {

                Physics::Position position{
                        corner +
                        (Physics::Position{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)}
                         * spacing)
                };

                Graphics::Color color{
                        y / fsize.y + x / fsize.x,
                        x / fsize.x,
                        1.0f - y / fsize.y + x / fsize.x
                };

                auto body = simulation.createBody();
                simulation.emplace_or_replace<Physics::Position>(body, position);
                simulation.emplace_or_replace<Physics::Velocity>(body, velocity);
                simulation.emplace_or_replace<Physics::ActiveMass>(body, mass);
                simulation.emplace_or_replace<Graphics::Color>(body, color);
                simulation.emplace_or_replace<Graphics::Sphere>(body, 2.0f);
            }
        }
    }
}

int main() {

    Simulation::Simulation simulation;
    cubicGrid(simulation, {0, 0, 0}, {3, 3, 3}, 10.0, {0, 0, 0}, 1.0);
    std::cout << simulation << std::endl;
}

/*
#include "../src/Model/Simulation.h"

Simulation cubicGrid(Position cornerPosition, glm::vec3 size, Velocity velocity, float spacing, float mass) {
    Simulation simulation{};

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {

                glm::vec3 coordinate(x, y, z);
                Model::Position p = cornerPosition + (glm::vec3{x, y, z} * spacing);

                Model::Graphics::Color color{
                        y / size.y + x / size.x,
                        x / size.x,
                        1.0f - y / size.y + x / size.x
                };

                simulation.newEntity()
                        .setPosition(p)
                        .setVelocity({velocity})
                        .setDrawable({color, 2})
                        .setActiveElement({mass})
                        .setPassiveElement({mass});
            }
        }
    }

    return simulation;
}

Simulation threeBody() {

    Simulation simulation{};

    simulation.newEntity()
            .setPosition({0, 5, 5})
            .setVelocity({0, -0.1, 0})
            .setDrawable({{0.8, 0.8, 0.8}, 1.0})
            .setPassiveElement({0.5})
            .setActiveElement({0.5});
    simulation.newEntity()
            .setPosition({-5, 0, 0})
            .setVelocity({0, 0.15, 0})
            .setDrawable({{0.8, 0.8, 0.0}, 1.0})
            .setPassiveElement({0.5})
            .setActiveElement({0.5});
    simulation.newEntity()
            .setPosition({5, 0, 0})
            .setVelocity({0, -0.05, 0})
            .setDrawable({{0.8, 0.0, 0.0}, 1.0})
            .setPassiveElement({0.5})
            .setActiveElement({0.5});

    return simulation;
}

Simulation galaxy() {

    auto simulation = cubicGrid(
            {-50, -150, -400},
            {5, 5, 500},
            {60, 20, 0},
            10, 5000);

    simulation._rule._gravitationalConstant = 0.01;
    simulation._rule._timeIncrement = 0.001;

    simulation.newEntity()
            .setPosition({0, 0, -500})
            .setVelocity({0, 0, 0})
            .setDrawable({{0.9, 0.9, 0.9}, 16.0})
            .setActiveElement({190000000.0f});

    return simulation;
}

Simulation blender() {

    Simulation simulation =
            cubicGrid(
                    {-50, -100, -450},
                    {5, 5, 100},
                    {0.8, 0.25, 0},
                    10, 5000);

    simulation._rule._gravitationalConstant = 0.01;
    simulation._rule._timeIncrement = 0.05;

    simulation.newEntity()
            .setPosition({-60, 0, -500})
            .setVelocity({0, -90, 0})
            .setDrawable({{0.9, 0.9, 0.9}, 16.0})
            .setPassiveElement({180000000})
            .setActiveElement({180000000});
    simulation.newEntity()
            .setPosition({60, 0, -500})
            .setVelocity({0, 90, 0})
            .setDrawable({{0.9, 0.9, 0.9}, 16.0})
            .setPassiveElement({180000000})
            .setActiveElement({180000000});

    return simulation;
}
*/