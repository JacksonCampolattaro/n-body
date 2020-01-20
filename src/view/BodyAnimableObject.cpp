//
// Created by jackcamp on 1/8/20.
//

#include "BodyAnimableObject.h"


BodyAnimableObject::BodyAnimableObject(Object3D *parent, SceneGraph::AnimableGroup3D* group, Body *body) :
        Object3D{parent},
        SceneGraph::Animable3D{*this, group},
        body(body) {


}

void BodyAnimableObject::animationStep(Float, Float delta) {

    setTranslation(Vector3(body->getPosition()));
}

void BodyAnimableObject::update() {

    setTranslation(Vector3(body->getPosition()));
}
