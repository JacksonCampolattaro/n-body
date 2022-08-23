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

    auto targets = _simulation.group<const Position, const Mass, Velocity>(entt::get<PassiveTag>);
    auto movableTargets = _simulation.group<Position, const Mass, const Velocity>(entt::get<PassiveTag>);

    // Construct an octree from the actor particles
    _statusDispatcher.emit({"Building Octree"});
    _octree->build(_maxDepth, _maxLeafSize);

    // Use the octree to estimate forces on each passive particle, and update their velocity
    _statusDispatcher.emit({"Computing Velocities"});
    tbb::parallel_for_each(targets, [&](Entity e) {
        const auto &targetPosition = targets.get<const Position>(e);
        const auto &targetMass = targets.get<const Mass>(e);
        auto &velocity = targets.get<Velocity>(e);

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

        tbb::parallel_for_each(movableTargets, [&](Entity e) {
            const auto &v = movableTargets.get<const Velocity>(e);
            auto &p = movableTargets.get<Position>(e);
            p = p + (v * _dt);
        });
    }

}
