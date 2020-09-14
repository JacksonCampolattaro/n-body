//
// Created by jackcamp on 9/13/20.
//

#include "Naive.h"

void Model::Solver::Naive::operator()(std::deque<Model::Physics::ActiveElement> &activeElements) {

    _activeElements = activeElements;
}

glm::vec3 Model::Solver::Naive::operator()(Model::Physics::PassiveElement passiveElement) const {

    return glm::vec3();
}
