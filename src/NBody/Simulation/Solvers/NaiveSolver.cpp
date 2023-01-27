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
using NBody::Physics::ActiveTag;
using NBody::Physics::PassiveTag;
using NBody::Physics::PassiveMass;
using NBody::Physics::Acceleration;

void NBody::NaiveSolver::step() {

    {
        _statusDispatcher.emit({"Resetting accelerations"});
        auto view = _simulation.view<const Position, const PassiveTag>();
        for (const Entity e: view)
            _simulation.emplace_or_replace<Acceleration>(e, 0.0f, 0.0f, 0.0f);
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

    // Update velocities, based on acceleration
    {
        _statusDispatcher.emit({"Updating velocities"});
        auto view = _simulation.view<const Acceleration, Velocity>();
        tbb::parallel_for_each(view, [&](Entity e) {
            const auto &a = view.template get<const Acceleration>(e);
            auto &v = view.template get<Velocity>(e);
            v = v + (a * _dt);
        });
    }

    // Update positions, based on velocity
    {
        // While the solver is modifying simulation values, the simulation should be locked for other threads
        std::scoped_lock l(_simulation.mutex);

        _statusDispatcher.emit({"Updating positions"});
        auto view = _simulation.view<const Velocity, Position>();

        tbb::parallel_for_each(view, [&](Entity e) {
            const auto &v = view.template get<const Velocity>(e);
            auto &p = view.template get<Position>(e);
            p = p + (v * _dt);
        });
    }

    // Other constraints (e.g. non-intersection/collision detection) should be enforced here
    // ...
}
