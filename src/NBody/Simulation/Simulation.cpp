//
// Created by jackcamp on 12/22/20.
//

#include "Simulation.h"

NBody::Simulation::Simulation::Simulation() {

    _bodies.add_normal_map();

    _bodies.add_property_map("color", NBody::Simulation::Color());
    _bodies.add_property_map("mass", 0);

    _bodies.insert({1, 2, 3}, {4, 5, 6});
}
