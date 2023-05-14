//
// Created by Jackson Campolattaro on 12/30/22.
//

#ifndef N_BODY_NSTEPSRUNNER_H
#define N_BODY_NSTEPSRUNNER_H

#include "../Runner.h"

namespace NBody {

    template<RuleType Rule = Gravity>
    class NStepsRunner : public Runner<Rule> {
    private:

        sigc::connection _idler;

        std::size_t _nSteps = 0;
        std::size_t _completedSteps = 0;

    public:

        explicit NStepsRunner(Solver<Rule> &solver) : Runner<Rule>(solver) {
            solver.signal_finished().connect([this]() {
                _completedSteps++;
            });
        }

        std::string id() override { return "n-steps"; };

        std::string name() override { return "Run N Steps"; };

        void start(std::size_t n) {
            spdlog::debug("Running {} steps", n);

            _nSteps = n;
            _completedSteps = 0;

            _idler = Glib::signal_timeout().connect([this] {

                if (_completedSteps < _nSteps)
                    this->solver().slot_step()();
                else
                    stop();

                return true;
            }, 1);
        }

        void stop() {
            _idler.disconnect();
            _completedSteps = 0;
            signal_done.emit();
        }

        std::size_t completedSteps() { return _completedSteps; }

    public:

        sigc::signal<void()> signal_done;
    };

}


#endif //N_BODY_NSTEPSRUNNER_H
