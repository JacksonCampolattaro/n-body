//
// Created by Jackson Campolattaro on 12/30/22.
//

#ifndef N_BODY_CONTINUOUSRUNNER_H
#define N_BODY_CONTINUOUSRUNNER_H

#include "../Runner.h"

namespace NBody {

    class ContinuousRunner : public Runner {
    private:

        sigc::connection _idler;

    public:

        explicit ContinuousRunner(Solver &solver) : Runner(solver) {}

        std::string id() override { return "continuous"; };

        std::string name() override { return "Run Continuously"; };

        void start();

        void stop();
    };

}


#endif //N_BODY_CONTINUOUSRUNNER_H
