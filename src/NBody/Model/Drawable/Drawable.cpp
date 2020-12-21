//
// Created by jackcamp on 4/17/20.
//

#include "Drawable.h"

Model::Drawable::Drawable::Drawable(Color color, float radius) :
        _position(nullptr),
        _color(color),
        _radius(radius) {

}

const Model::Position &Model::Drawable::Drawable::position() const {
    return *_position;
}
