//
// Created by Jackson Campolattaro on 7/2/22.
//

#ifndef N_BODY_BARNESHUTSOLVER_H
#define N_BODY_BARNESHUTSOLVER_H

#include "../Solver.h"

#include "Trees/DescentCriterion.h"

#include "Trees/Octree.h"
#include "Trees/LinearBVH.h"

#include <tbb/parallel_for_each.h>

#include <span>
#include <memory>

namespace NBody {

    template<typename TreeType, typename DescentCriterionType>
    class BarnesHutSolverBase : public Solver {
    private:

        std::unique_ptr<TreeType> _tree;
        DescentCriterionType _descentCriterion{0.4f};

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
                        _descentCriterion,
                        simulation().template view<const Position, const Mass, const ActiveTag>(),
                        targetPosition,
                        targetMass,
                        _rule
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

        float &theta() { return _descentCriterion.theta(); }

        const float &theta() const { return _descentCriterion.theta(); }

    protected:

        inline Acceleration computeAcceleration(const typename TreeType::Node &node,
                                                DescentCriterionType &descentCriterion,
                                                const entt::basic_view<
                                                        entt::entity, entt::exclude_t<>,
                                                        const Position, const Mass, const ActiveTag
                                                > &activeParticles,
                                                const Position &passivePosition,
                                                const Mass &passiveMass,
                                                const Rule &rule) {

            // Empty nodes can be ignored
            if (node.contents().empty()) return Physics::Acceleration{};

            if (descentCriterion(node, passivePosition)) {

                // Node is treated as a single particle if S/D < theta (where S = sideLength and D = distance)
                return rule(node.centerOfMass(), node.totalMass(),
                            passivePosition, passiveMass);

            } else {

                // Otherwise, the node can't be summarized
                if (node.isLeaf()) {

                    // If this is a leaf node, interact with all particles contained
                    return std::transform_reduce(
                            node.contents().begin(), node.contents().end(),
                            Physics::Acceleration{}, std::plus{},
                            [&](auto entity) {
                                return rule(activeParticles.get<const Position>(entity),
                                            activeParticles.get<const Mass>(entity),
                                            passivePosition, passiveMass);
                            }
                    );

                } else {

                    // If it's a non-leaf node, descend the tree (recursive case)
                    return std::transform_reduce(
                            node.children().begin(), node.children().end(),
                            Physics::Acceleration{}, std::plus{},
                            [&](const auto &child) {
                                return computeAcceleration(
                                        child, descentCriterion,
                                        activeParticles,
                                        passivePosition, passiveMass,
                                        rule
                                );
                            }
                    );
                }
            }
        }

    };

    class BarnesHutSolver : public BarnesHutSolverBase<Octree, DescentCriterion::SideLengthOverDistance> {
    public:

        BarnesHutSolver(Simulation &simulation, Physics::Rule &rule) :
                BarnesHutSolverBase<Octree, DescentCriterion::SideLengthOverDistance>(simulation, rule) {}

        std::string id() override { return "barnes-hut"; };

        std::string name() override { return "Barnes-Hut"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

}


#endif //N_BODY_BARNESHUTSOLVER_H
