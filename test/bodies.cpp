
#include "../src/Model/Simulation.h"

using Model::Simulation;

void threeBody() {

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
    simulation.saveBodiesToPath("../../scenarios/threeBody.bod");
}

void galaxy() {

    Simulation simulation{};

    // "Sun"
    simulation.newEntity()
            .setPosition({0, 0, -200})
            .setVelocity({0, 0, 0})
            .setDrawable({{0.9, 0.9, 0.9}, 10.0})
            .setActiveElement({500});

    // "Asteroids"
    glm::vec3 dimensions{5, 5, 5};
    Model::Position corner{-10, -10, -10};
    float spacing = 2;
    for (int x = 0; x < dimensions.x; ++x) {
        for (int y = 0; y < dimensions.y; ++y) {
            for (int z = 0; z < dimensions.z; ++z) {

                Model::Position p = corner + (glm::vec3{x, y, z} * spacing);

                simulation.newEntity()
                        .setPosition(p)
                        .setVelocity({0.8, 0.25, 0})
                        .setDrawable({{0.0, 0.9, 0.9}, 1})
                        .setActiveElement({0.5})
                        .setPassiveElement({0.5});
            }
        }
    }

    simulation.saveBodiesToPath("../../scenarios/galaxy.bod");
}

int main() {

    threeBody();
    galaxy();
    return 0;
}