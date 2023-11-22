//
// Created by Jackson Campolattaro on 11/22/23.
//

#ifndef N_BODY_FIELDRENDERER_H
#define N_BODY_FIELDRENDERER_H

#include <NBody/Simulation/Simulation.h>
#include <NBody/Simulation/Solver.h>

#include "../Renderer.h"

namespace NBody {

    using namespace Magnum;
    using namespace Math::Literals;

    template<RuleType Rule = Gravity>
    class FieldRenderer : public Renderer {
    public:

        FieldRenderer(Solver<Rule> &solver) : Renderer(), _solver(solver) {}

        void draw(const Matrix4 &transformationMatrix, const Matrix4 &projectionMatrix) override {

            if (!_enabled)
                return;

            spdlog::info("Drawing the field");

            // todo
        }

        sigc::signal<void()> &signal_changed() override { return _signal_changed; };

        void setEnabled(bool enabled) {
            _enabled = enabled;
            _signal_changed.emit();
        }

        const bool &enabled() const { return _enabled; }

    private:

        Solver<Rule> &_solver;
        bool _enabled = false;

        mutable sigc::signal<void()> _signal_changed;

    };

}


#endif //N_BODY_FIELDRENDERER_H
