#include <cassert>

#include "../src/Model/Simulation.h"
#include "../src/Model/Entity.h"

using Model::Simulation;
using Model::Entity;

int main() {

    Simulation simulation{};

    auto original = simulation.newEntity();
    original.setPosition({1, 2, 3})
            .setVelocity({4, 5, 6})
            .setDrawable({{0.1, 0.2, 0.3}, 100})
            .setPassiveElement({200})
            .setActiveElement({300});

}
