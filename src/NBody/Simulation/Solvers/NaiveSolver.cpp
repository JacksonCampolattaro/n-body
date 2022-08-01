//
// Created by Jackson Campolattaro on 19/04/2022.
//

#include "NaiveSolver.h"

#include <glm/glm.hpp>

#include <tbb/tbb.h>
#include <tbb/blocked_range.h>

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::Mass;
using NBody::Physics::ActiveTag;
using NBody::Physics::PassiveTag;
using NBody::Physics::PassiveMass;

void NBody::NaiveSolver::step() {

    auto actors = _simulation.group<const Position, const Mass>(entt::get<ActiveTag>);
    auto targets = _simulation.group<const Position, const Mass, Velocity>(entt::get<PassiveTag>);
    auto movableTargets = _simulation.group<Position, const Mass, const Velocity>(entt::get<PassiveTag>);

    // Update accelerations and velocities
    {
        spdlog::trace("Updating velocities");
        _statusDispatcher.emit({"Computing velocities"});
        tbb::parallel_for_each(targets, [&](Entity e) {
            const auto &targetPosition = targets.get<const Position>(e);
            const auto &targetMass = targets.get<const Mass>(e);
            auto &velocity = targets.get<Velocity>(e);

            actors.each([&](const Position &actorPosition, const Mass &actorMass) {

                if (actorPosition == targetPosition) return;

                glm::vec3 acceleration = _rule(actorPosition, actorMass, targetPosition, targetMass);

                velocity += acceleration * _dt;
            });
        });
    }

    // Update positions, based on velocity
    {
        // While the solver is modifying simulation values, the simulation should be locked for other threads
        std::scoped_lock l(_simulation.mutex);

        spdlog::trace("Updating positions");
        _statusDispatcher.emit({"Updating Positions"});

        tbb::parallel_for_each(movableTargets, [&](Entity e) {
            const auto &v = movableTargets.get<const Velocity>(e);
            auto &p = movableTargets.get<Position>(e);
            p = p + (v * _dt);
        });
    }

    // Other constraints (e.g. non-intersection/collision detection) should be enforced here
    // ...
}
