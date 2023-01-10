//
// Created by Jackson Campolattaro on 7/2/22.
//

#ifndef N_BODY_BARNESHUTSOLVER_H
#define N_BODY_BARNESHUTSOLVER_H

#include "../Solver.h"

#include "Octree.h"
#include "LinearBVH.h"

#include <tbb/parallel_for_each.h>

#include <span>
#include <memory>

namespace NBody {

    template<typename TreeType>
    class BarnesHutSolverBase : public Solver {
    private:

        std::unique_ptr<TreeType> _tree;
        float _theta = 0.4f;

    public:

        BarnesHutSolverBase(Simulation &simulation, Physics::Rule &rule) :
                Solver(simulation, rule),
                _tree(std::make_unique<TreeType>(simulation)) {}

        void step() override {

            auto targets = _simulation.view<const Position, const Mass, Velocity, const PassiveTag>();
            auto movableTargets = _simulation.view<Position, const Mass, const Velocity>();

            // Construct an octree from the actor particles
            _statusDispatcher.emit({"Building Tree"});
            _tree->refine();

            // Use the octree to estimate forces on each passive particle, and update their velocity
            _statusDispatcher.emit({"Computing Velocities"});
            tbb::parallel_for_each(targets, [&](Entity e) {
                const auto &targetPosition = targets.template get<const Position>(e);
                const auto &targetMass = targets.template get<const Mass>(e);
                auto &velocity = targets.template get<Velocity>(e);

                assert(_tree);
                auto acceleration = computeAcceleration(
                        _tree->root(),
                        simulation().template view<const Position, const Mass, const ActiveTag>(),
                        targetPosition,
                        targetMass,
                        _rule,
                        _theta
                );

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
                    const auto &v = movableTargets.template get<const Velocity>(e);
                    auto &p = movableTargets.template get<Position>(e);
                    p = p + (v * _dt);
                });
            }

        };

        TreeType &tree() { return *_tree; }

        const TreeType &tree() const { return *_tree; }

        float &theta() { return _theta; }

        const float &theta() const { return _theta; }

    };

    class BarnesHutSolver : public BarnesHutSolverBase<Octree> {
    public:

        BarnesHutSolver(Simulation &simulation, Physics::Rule &rule) :
                BarnesHutSolverBase<Octree>(simulation, rule) {}

        std::string id() override { return "barnes-hut"; };

        std::string name() override { return "Barnes-Hut"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    class LinearBVHSolver : public BarnesHutSolverBase<LinearBVH> {
    public:

        LinearBVHSolver(Simulation &simulation, Physics::Rule &rule) :
                BarnesHutSolverBase<LinearBVH>(simulation, rule) {}

        std::string id() override { return "linear-bvh"; };

        std::string name() override { return "Linear-BVH"; };

    };

}


#endif //N_BODY_BARNESHUTSOLVER_H
