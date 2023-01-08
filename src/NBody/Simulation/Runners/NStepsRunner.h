//
// Created by Jackson Campolattaro on 12/30/22.
//

#ifndef N_BODY_NSTEPSRUNNER_H
#define N_BODY_NSTEPSRUNNER_H

#include "../Runner.h"

namespace NBody {

    class NStepsRunner : public Runner {
    private:

        sigc::connection _idler;

        std::size_t _nSteps = 0;
        std::size_t _completedSteps = 0;

    public:

        explicit NStepsRunner(Solver &solver) : Runner(solver) {
            solver.signal_finished().connect([this]() {
                _completedSteps++;
            });
        }

        std::string id() override { return "n-steps"; };

        std::string name() override { return "Run N Steps"; };

        void start(std::size_t n);

        void stop();

        std::size_t completedSteps() { return _completedSteps; }

    public:

        sigc::signal<void()> signal_done;
    };

}


#endif //N_BODY_NSTEPSRUNNER_H
