//
// Created by Jackson Campolattaro on 12/29/22.
//

#ifndef N_BODY_RUNNER_H
#define N_BODY_RUNNER_H

#include "Solver.h"

namespace NBody {

    template<RuleType Rule = Gravity>
    class Runner : public Glib::Object {
    protected:

        Solver<Rule> &_solver;

    public:

        explicit Runner(Solver<Rule> &solver) :
                Glib::ObjectBase(typeid(NBody::Runner<Rule>)), Glib::Object(),
                _solver(solver) {}

        virtual std::string id() = 0;

        virtual std::string name() = 0;

        Solver<Rule> &solver() { return _solver; }

        const Solver<Rule> &solver() const { return _solver; }

    };

}

#endif //N_BODY_RUNNER_H
