//
// Created by Jackson Campolattaro on 19/04/2022.
//

#include "NaiveSolver.h"

#include <glm/glm.hpp>

#include <tbb/tbb.h>
#include <tbb/blocked_range.h>

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::ActiveMass;
using NBody::Physics::PassiveMass;

void NBody::NaiveSolver::step() {

    // Update accelerations and velocities
    {
        spdlog::debug("Updating velocities");
        auto actors = _simulation.view<const Position, const ActiveMass>();
        auto targets = _simulation.view<const Position, const PassiveMass, Velocity>();
        tbb::parallel_for_each(targets, [&](Entity e) {
            const auto &targetPosition = _simulation.get<Position>(e);
            const auto &targetMass = _simulation.get<PassiveMass>(e);
            auto &velocity = _simulation.get<Velocity>(e);

            actors.each([&](const Position &actorPosition, const ActiveMass &actorMass) {

                if (actorPosition == targetPosition) return;

                glm::vec3 acceleration = _rule(actorPosition, actorMass, targetPosition, targetMass);

                velocity += acceleration * _dt;
            });
        });
    }

    // Update positions, based on velocity
    {
        spdlog::debug("Updating positions");
        auto view = _simulation.view<const Velocity, Position>();

        tbb::parallel_for_each(view, [&](Entity e) {
            const auto &v = _simulation.get<Velocity>(e);
            auto &p = _simulation.get<Position>(e);
            p = p + (v * _dt);
        });
    }

    // Other constraints (e.g. non-intersection/collision detection) should be enforced here
}
