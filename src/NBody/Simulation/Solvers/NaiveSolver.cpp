//
// Created by Jackson Campolattaro on 19/04/2022.
//

#include "NaiveSolver.h"

using NBody::Physics::Position;
using NBody::Physics::Velocity;


void NBody::NaiveSolver::step() {

    // Update velocities
    // todo: This is a temporary example, in the future a Rule should be used here
    {
        spdlog::debug("Updating velocities");
        auto view = _simulation.view<const Position, Velocity>();
        view.each([](const Position &position, Velocity &velocity){
            // todo
        });
    }

    // Update positions, based on velocity
    {
        spdlog::debug("Updating positions");
        auto view = _simulation.view<const Velocity, Position>();
        view.each([&](const Velocity &v, Position &p){
            p = p + (v * _dt);
        });
    }

    // Other constraints (e.g. non-intersection/collision detection) should be enforced here
}
