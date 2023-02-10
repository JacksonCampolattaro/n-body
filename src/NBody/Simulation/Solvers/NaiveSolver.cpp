//
// Created by Jackson Campolattaro on 19/04/2022.
//

#include "NaiveSolver.h"

#include <execution>

#include <glm/glm.hpp>

#include <tbb/tbb.h>
#include <tbb/blocked_range.h>

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::Mass;
using NBody::Physics::Acceleration;

void NBody::NaiveSolver::updateAccelerations() {

    {
        _statusDispatcher.emit({"Resetting accelerations"});
        auto view = _simulation.view<Acceleration>();
        view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
    }

    // Compute accelerations
    {
        _statusDispatcher.emit({"Computing accelerations"});
        auto view = _simulation.view<const Position, Acceleration>();
        auto actorsView = _simulation.view<const Position, const Mass>();
        tbb::parallel_for_each(view, [&](Entity e) {
            const auto &passivePosition = view.get<const Position>(e);
            auto &passiveAcceleration = view.get<Acceleration>(e);

            actorsView.each([&](const Position &activePosition, const Mass &activeMass) {
                passiveAcceleration += (glm::vec3) _rule(activePosition, activeMass, passivePosition);
            });
        });
    }
}
