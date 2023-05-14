//
// Created by Jackson Campolattaro on 12/30/22.
//

#ifndef N_BODY_CONTINUOUSRUNNER_H
#define N_BODY_CONTINUOUSRUNNER_H

#include "../Runner.h"

namespace NBody {

    template<RuleType Rule = Gravity>
    class ContinuousRunner : public Runner<Rule> {
    private:

        sigc::connection _idler;

    public:

        explicit ContinuousRunner(Solver<Rule> &solver) : Runner<Rule>(solver) {}

        std::string id() override { return "continuous"; };

        std::string name() override { return "Run Continuously"; };

        void start() {
            spdlog::debug("Running continuously");

            _idler = Glib::signal_timeout().connect([this] {

                // todo: this shouldn't be a slot
                this->solver().slot_step()();

                return true;
            }, 1);
        }

        void stop() {
            _idler.disconnect();
        }
    };

}


#endif //N_BODY_CONTINUOUSRUNNER_H
