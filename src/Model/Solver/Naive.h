//
// Created by jackcamp on 9/13/20.
//

#ifndef N_BODY_NAIVE_H
#define N_BODY_NAIVE_H

#include "Solver.h"

namespace Model {
    namespace Solver {

        class Naive : public Solver::Solver {
        public:

            Naive() = default;

            void operator()(std::deque<Model::Physics::ActiveElement> &activeElements);

            glm::vec3 operator()(Model::Physics::PassiveElement passiveElement) const;

        private:

            std::deque<Model::Physics::ActiveElement> &_activeElements;
        };

    }
}

#endif //N_BODY_NAIVE_H
