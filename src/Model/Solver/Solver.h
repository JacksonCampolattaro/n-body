//
// Created by jackcamp on 9/13/20.
//

#ifndef N_BODY_SOLVER_H
#define N_BODY_SOLVER_H

#include "../Physics/PassiveElement.h"
#include "../Physics/ActiveElement.h"

#include <glm/vec3.hpp>
#include <deque>

namespace Model {

    class Solver {
    public:

        virtual void operator()(const std::deque<Model::Physics::ActiveElement> &activeElements) = 0;

        virtual glm::vec3 operator()(Model::Physics::PassiveElement passiveElement) const = 0;
    };
}

#endif //N_BODY_SOLVER_H
