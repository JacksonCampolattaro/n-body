//
// Created by jackcamp on 4/17/20.
//

#include "Rule.h"

#include <glm/geometric.hpp>
#include <cmath>

Model::Physics::Rule::Rule() :
        _gravitationalConstant(0.00000000001),
        _timeIncrement(0.01) {
}

void
Model::Physics::Rule::operator()(const Model::Physics::ActiveElement &active, Model::Physics::PassiveElement &passive) {

    float forceMagnitude =
            (_gravitationalConstant * passive._mass * active._mass) /
            ((float) pow((float) glm::distance((glm::vec3) passive.position(), (glm::vec3) active.position()), 2.0f) +
             0.0001);

    std::cout << forceMagnitude << std::endl;

    glm::vec3 force = forceMagnitude * glm::normalize(active.position() - passive.position());

    glm::vec3 acceleration = force / passive._mass;

    passive.velocity() += acceleration * _timeIncrement;

//    (*passive._velocityVector)[passive._velocityIndex].x += 0.5;
}
