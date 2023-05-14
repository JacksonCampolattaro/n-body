//
// Created by Jackson Campolattaro on 12/30/22.
//

#ifndef N_BODY_ONESTEPRUNNER_H
#define N_BODY_ONESTEPRUNNER_H

#include "../Runner.h"

namespace NBody {

    template<RuleType Rule = Gravity>
    class OneStepRunner : public Runner<Rule> {
    public:

        explicit OneStepRunner(Solver<Rule> &solver) : Runner<Rule>(solver) {}

        std::string id() override { return "one-step"; };

        std::string name() override { return "Run One Step"; };

        void step() {

            spdlog::debug("Running one step");

            // todo: this shouldn't be a slot
            this->solver().slot_step()();
        }

    };

}

#endif //N_BODY_ONESTEPRUNNER_H
