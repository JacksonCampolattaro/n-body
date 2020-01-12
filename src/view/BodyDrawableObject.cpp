//
// Created by jackcamp on 1/8/20.
//

#include "BodyDrawableObject.h"

BodyDrawableObject::BodyDrawableObject(Object3D *parent, Body *body) :
        Object3D{parent},
        body(body) {}

void BodyDrawableObject::update() {

    Matrix4 transformation = Matrix4::translation(
            {body->getPosition().x, body->getPosition().y, body->getPosition().z}
    );

    setTransformation(transformation);
    ///setDirty();
}
