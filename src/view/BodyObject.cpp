//
// Created by jackcamp on 1/8/20.
//

#include "BodyObject.h"


BodyObject::BodyObject(Object3D *parent, Body *body) :
        Object3D{parent},
        body(body) {

}

void BodyObject::update() {

    translate(Vector3(body->getPosition()));
}
