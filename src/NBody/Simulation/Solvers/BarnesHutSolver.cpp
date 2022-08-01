//
// Created by Jackson Campolattaro on 7/2/22.
//

#include "BarnesHutSolver.h"

#include <tbb/tbb.h>
#include <tbb/blocked_range.h>

using NBody::Physics::Position;
using NBody::Physics::Velocity;
using NBody::Physics::Mass;
using NBody::Physics::PassiveTag;
using NBody::Physics::ActiveTag;

void NBody::BarnesHutSolver::step() {

    // Construct an octree from the actor particles
    _statusDispatcher.emit({"Building Octree"});
    _octree->build(_maxDepth, _maxLeafSize);

    // Use the octree to estimate forces on each passive particle, and update their velocity
    _statusDispatcher.emit({"Computing Velocities"});
    auto targets = _simulation.view<const Position, const Mass, Velocity, PassiveTag>();
    tbb::parallel_for_each(targets, [&](Entity e) {
        const auto &targetPosition = _simulation.get<Position>(e);
        const auto &targetMass = _simulation.get<Mass>(e);
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
        _statusDispatcher.emit({"Updating positions"});
        auto view = _simulation.view<const Velocity, Position>();

        tbb::parallel_for_each(view, [&](Entity e) {
            const auto &v = _simulation.get<Velocity>(e);
            auto &p = _simulation.get<Position>(e);
            p = p + (v * _dt);
        });
    }

}
