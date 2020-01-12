//
// Created by jackcamp on 1/8/20.
//

#include "BodyObject.h"

BodyObject::BodyObject(Object3D *parent, Body *body) :
        Object3D{parent},
        body(body) {}

void BodyObject::update() {

    /*Matrix4 translation = Matrix4::translation(
            {body->getPosition().x, body->getPosition().y, body->getPosition().z}
    );*/

    translate({body->getPosition().x, body->getPosition().y, body->getPosition().z});
}
