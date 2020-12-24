//
// Created by jackcamp on 12/23/20.
//

#include "Drawable.h"

NBody::Simulation::Drawable::Drawable(NBody::Simulation::Color color, float radius) :
        _color(color),
        _radius(radius) {

}

NBody::Simulation::Color &NBody::Simulation::Drawable::color() {
    return _color;
}

const NBody::Simulation::Color &NBody::Simulation::Drawable::color() const {
    return _color;
}

float &NBody::Simulation::Drawable::radius() {
    return _radius;
}

const float &NBody::Simulation::Drawable::radius() const {
    return _radius;
}

void NBody::Simulation::to_json(json &j, const NBody::Simulation::Drawable &drawable) {

    j = {
            {"color", drawable.color()},
            {"radius", drawable.radius()}
    };
}

void NBody::Simulation::from_json(const json &j, NBody::Simulation::Drawable &drawable) {

}

