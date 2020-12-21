//
// Created by jackcamp on 4/17/20.
//

#include "Rule.h"

#include <glm/geometric.hpp>
#include <cmath>

#include "../../../Application/Logger.h"

Model::Physics::Rule::Rule() :
        _gravitationalConstant(1),
        _timeIncrement(1) {
}

void
Model::Physics::Rule::operator()(const Model::Physics::ActiveElement &active, Model::Physics::PassiveElement &passive) const {

    if (active.position() == passive.position())
        return;

    float distance = glm::distance((glm::vec3) passive.position(), (glm::vec3) active.position());

    float forceMagnitude =
            (float) (_gravitationalConstant * passive._mass * active._mass) /
            (float) (distance * distance +
             0.0001f);

    glm::vec3 forceDirection = (active.position() - passive.position()) / distance;

    glm::vec3 force = forceMagnitude * forceDirection;

    glm::vec3 acceleration = force / passive._mass;

    passive.velocity() += acceleration * _timeIncrement;
}
