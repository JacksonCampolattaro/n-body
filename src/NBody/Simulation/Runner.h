//
// Created by Jackson Campolattaro on 12/29/22.
//

#ifndef N_BODY_RUNNER_H
#define N_BODY_RUNNER_H

#include "Solver.h"

namespace NBody {

    class Runner : public Glib::Object {
    protected:

        Solver &_solver;

    public:

        explicit Runner(Solver &solver) :
                Glib::ObjectBase(typeid(NBody::Runner)), Glib::Object(),
                _solver(solver) {}

        virtual std::string id() = 0;

        virtual std::string name() = 0;

        Solver &solver() { return _solver; }

        const Solver &solver() const { return _solver; }

    };

}

#endif //N_BODY_RUNNER_H
