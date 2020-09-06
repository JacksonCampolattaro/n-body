//
// Created by jackcamp on 4/17/20.
//

#include "Rule.h"

#include <glm/geometric.hpp>
#include <cmath>

#include "../../Controller/Logger.h"

Model::Physics::Rule::Rule() :
        _gravitationalConstant(1),
        _timeIncrement(1) {
}

void
Model::Physics::Rule::operator()(const Model::Physics::ActiveElement &active, Model::Physics::PassiveElement &passive) const {

    spdlog::trace("Rule invoked");

    float forceMagnitude =
            (float) (_gravitationalConstant * passive._mass * active._mass) /
            (float) (pow((float) glm::distance((glm::vec3) passive.position(), (glm::vec3) active.position()), 2.0f) +
             0.0001);

    glm::vec3 forceDirection = glm::normalize(active.position() - passive.position());

    glm::vec3 force = forceMagnitude * glm::normalize(active.position() - passive.position());

    glm::vec3 acceleration = force / passive._mass;

    passive.velocity() += acceleration * _timeIncrement;
}
