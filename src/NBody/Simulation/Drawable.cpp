//
// Created by jackcamp on 12/23/20.
//

#include "Drawable.h"

NBody::Simulation::Drawable::Drawable(NBody::Simulation::Color color, float radius) :
        _color(color),
        _radius(radius) {

}

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Drawable &drawable) {

}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::Drawable &drawable) {

}

