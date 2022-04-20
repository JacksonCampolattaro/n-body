//
// Created by Jackson Campolattaro on 19/04/2022.
//

#include "NaiveSolver.h"

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::ActiveMass;
using NBody::Physics::PassiveMass;

#include <glm/glm.hpp>

void NBody::NaiveSolver::step() {

    // Update velocities
    // todo: This is a temporary example, in the future a Rule should be used here
    {
        spdlog::debug("Updating velocities");
        auto actors = _simulation.view<const Position, const ActiveMass>();
        auto targets = _simulation.view<const Position, const PassiveMass, Velocity>();
        targets.each([&](const Position &targetPosition, const PassiveMass &targetMass, Velocity &velocity) {

            actors.each([&](const Position &actorPosition, const ActiveMass &actorMass) {

                if (actorPosition == targetPosition) return;

                float force = 1.0f * targetMass.mass() * actorMass.mass() /
                              (glm::length(actorPosition - targetPosition) + 0.0001);

                glm::vec3 forceVector = glm::normalize(actorPosition - targetPosition) * force;

                glm::vec3 acceleration = forceVector / targetMass.mass();

                velocity += acceleration * _dt;
            });
        });
    }

    // Update positions, based on velocity
    {
        spdlog::debug("Updating positions");
        auto view = _simulation.view<const Velocity, Position>();
        view.each([&](const Velocity &v, Position &p) {
            p = p + (v * _dt);
        });
    }

    // Other constraints (e.g. non-intersection/collision detection) should be enforced here
}
