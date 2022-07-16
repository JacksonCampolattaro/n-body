//
// Created by Jackson Campolattaro on 7/2/22.
//

#include "BarnesHutSolver.h"

#include <tbb/tbb.h>
#include <tbb/blocked_range.h>

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::ActiveMass;
using NBody::Physics::PassiveMass;

void NBody::BarnesHutSolver::step() {

    // Construct an octree from the actor particles
    // todo This doesn't actually need to be re-done from scratch every time!
    _octree = std::make_unique<Octree>(_simulation);

    // Use the octree to estimate forces on each passive particle, and update their velocity
    auto targets = _simulation.view<const Position, const PassiveMass, Velocity>();
    tbb::parallel_for_each(targets, [&](Entity e) {
        const auto &targetPosition = _simulation.get<Position>(e);
        const auto &targetMass = _simulation.get<PassiveMass>(e);
        auto &velocity = _simulation.get<Velocity>(e);

        assert(_octree);
        auto acceleration = _octree->applyRule(_rule, targetPosition, targetMass, _theta);

        velocity += acceleration * _dt;
    });

    // Update positions, based on velocity
    {
        // While the solver is modifying simulation values, the simulation should be locked for other threads
        std::scoped_lock l(_simulation.mutex);

        spdlog::trace("Updating positions");
        auto view = _simulation.view<const Velocity, Position>();

        tbb::parallel_for_each(view, [&](Entity e) {
            const auto &v = _simulation.get<Velocity>(e);
            auto &p = _simulation.get<Position>(e);
            p = p + (v * _dt);
        });
    }

}