//
// Created by jackcamp on 4/17/20.
//

#include "Sphere.h"

Model::Drawable::Sphere::Sphere(Color3 color, float radius) :
        _color(color),
        _radius(radius) {

    _position = std::make_shared<Position>(0, 0, 0);
}
