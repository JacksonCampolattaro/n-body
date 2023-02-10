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

        TreeType _tree;
        DescentCriterionType _descentCriterion{0.4f};

    public:

        BarnesHutSolverBase(Simulation &simulation, Physics::Rule &rule) :
                Solver(simulation, rule),
                _tree(simulation) {}

        void updateAccelerations() override {

            {
                // Construct an octree from the actor particles
                _statusDispatcher.emit({"Building Tree"});
                _tree.refine();
            }

            {
                _statusDispatcher.emit({"Resetting accelerations"});
                auto view = _simulation.view<Acceleration>();
                view.each([](Acceleration &acceleration) { acceleration = {0.0f, 0.0f, 0.0f}; });
            }

            {
                // Use the octree to estimate forces on each passive particle
                _statusDispatcher.emit({"Computing Accelerations"});
                auto view = _simulation.template view<const Position, Acceleration>();
                tbb::parallel_for_each(view, [&](Entity e) {
                    const auto &targetPosition = view.template get<const Position>(e);
                    auto &acceleration = view.template get<Acceleration>(e);

                    acceleration = computeAcceleration(
                            _tree.root(),
                            simulation().template view<const Position, const Mass>(),
                            targetPosition
                    );
                });
            }
        }

        TreeType &tree() { return _tree; }

        const TreeType &tree() const { return _tree; }

        float &theta() { return _descentCriterion.theta(); }

        const float &theta() const { return _descentCriterion.theta(); }

    protected:

        inline Acceleration computeAcceleration(const typename TreeType::Node &node,
                                                const entt::basic_view<
                                                        entt::entity, entt::exclude_t<>,
                                                        const Position, const Mass
                                                > &activeParticles,
                                                const Position &passivePosition) {

            // Empty nodes can be ignored
            if (node.contents().empty()) return Physics::Acceleration{};

            if (_descentCriterion(node, passivePosition)) {

                return _rule(node.summary().centerOfMass(), node.summary().totalMass(), passivePosition);

            } else {

                // Otherwise, the node can't be summarized
                if (node.isLeaf()) {

                    // If this is a leaf node, interact with all particles contained
                    return std::transform_reduce(
                            node.contents().begin(), node.contents().end(),
                            Physics::Acceleration{}, std::plus{},
                            [&](auto entity) {
                                return _rule(activeParticles.get<const Position>(entity),
                                             activeParticles.get<const Mass>(entity),
                                             passivePosition);
                            }
                    );

                } else {

                    // If it's a non-leaf node, descend the tree (recursive case)
                    return std::transform_reduce(
                            node.children().begin(), node.children().end(),
                            Physics::Acceleration{}, std::plus{},
                            [&](const auto &child) {
                                return computeAcceleration(
                                        child,
                                        activeParticles,
                                        passivePosition
                                );
                            }
                    );
                }
            }
        }

    };

    class BarnesHutSolver : public BarnesHutSolverBase<ActiveOctree, DescentCriterion::SideLengthOverDistance> {
    public:

        BarnesHutSolver(Simulation &simulation, Physics::Rule &rule) :
                BarnesHutSolverBase<ActiveOctree, DescentCriterion::SideLengthOverDistance>(simulation, rule) {}

        std::string id() override { return "barnes-hut"; };

        std::string name() override { return "Barnes-Hut"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

}


#endif //N_BODY_BARNESHUTSOLVER_H
