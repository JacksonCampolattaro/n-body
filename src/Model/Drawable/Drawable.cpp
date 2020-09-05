//
// Created by jackcamp on 4/17/20.
//

#include "Drawable.h"

Model::Drawable::Drawable::Drawable(Position *position, Color3 color, float radius) :
        _position(position),
        _color(color),
        _radius(radius) {

}

const Model::Position &Model::Drawable::Drawable::position() const {
    return *_position;
}
