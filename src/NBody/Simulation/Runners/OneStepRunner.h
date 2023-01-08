//
// Created by Jackson Campolattaro on 12/30/22.
//

#ifndef N_BODY_ONESTEPRUNNER_H
#define N_BODY_ONESTEPRUNNER_H

#include "../Runner.h"

namespace NBody {

    class OneStepRunner : public Runner{
    public:

        explicit OneStepRunner(Solver &solver) : Runner(solver) {}

        std::string id() override { return "one-step"; };

        std::string name() override { return "Run One Step"; };

        void step();

    };

}

#endif //N_BODY_ONESTEPRUNNER_H
