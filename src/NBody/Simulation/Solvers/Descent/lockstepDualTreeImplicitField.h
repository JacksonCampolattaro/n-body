//
// Created by Jackson Campolattaro on 5/5/23.
//

#ifndef N_BODY_LOCKSTEPDUALTREEIMPLICITFIELD_H
#define N_BODY_LOCKSTEPDUALTREEIMPLICITFIELD_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/activeTree.h>
#include <NBody/Simulation/Solvers/Descent/passiveTreeImplicitField.h>
#include <NBody/Simulation/Solvers/Descent/none.h>

#include <NBody/Physics/Summaries/EmptySummary.h>

#include <queue>

namespace NBody::Descent {

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion>
    inline std::tuple<
            std::span<std::reference_wrapper<const ActiveNode>>,
            std::span<std::reference_wrapper<const ActiveNode>>,
            std::span<std::reference_wrapper<const ActiveNode>>
    > partitionByDescentCriterionLockstep(
            std::span<std::reference_wrapper<const ActiveNode>> activeNodes,
            PassiveNode &passiveNode, const DescentCriterion &descentCriterion
    ) {

        auto endFarNodes = activeNodes.begin();
        auto beginNonLeafNodes = activeNodes.end();

        for (auto i = activeNodes.begin(); i < beginNonLeafNodes; ++i) {
            auto &activeNode = (*i).get();
            auto recommendation = descentCriterion(activeNode, passiveNode);

            if (recommendation == Recommendation::Approximate) {

                std::iter_swap(i, endFarNodes);
                ++endFarNodes;

            } else if (!activeNode.isLeaf()) {

                --beginNonLeafNodes;
                std::iter_swap(i, beginNonLeafNodes);

                // we've swapped an unknown value into position i, so we'll need to re-examine it
                i--;
            }
        }

        return {
                {activeNodes.begin(), endFarNodes},
                {endFarNodes,         beginNonLeafNodes},
                {beginNonLeafNodes,   activeNodes.end()}
        };
    }

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion, RuleType Rule = Gravity>
    inline void lockstepDualTreeImplicitField(
            std::span<std::reference_wrapper<const ActiveNode>> activeNodes,
            PassiveNode &passiveNode,
            const DescentCriterion &descentCriterion, Rule &rule,
            const entt::basic_view<
                    Entity, entt::exclude_t<>,
                    const Position, const Mass
            > &activeContext,
            const entt::basic_view<
                    Entity, entt::exclude_t<>,
                    const Position, Acceleration
            > &passiveContext,
            typename PassiveNode::Summary::ImpliedSummary localField = {}
    ) {

        // Empty nodes can be ignored
        if (passiveNode.contents().empty()) return;

        // If there are no more forces to calculate, just apply the local field to the particles
        if (activeNodes.empty()) {
            for (auto passiveEntity: passiveNode.contents()) {
                passiveContext.get<Acceleration>(passiveEntity) += localField.acceleration().at(
                        passiveContext.get<const Position>(passiveEntity) - passiveNode.center()
                );
            }
            return;
        }

        // Begin by sorting the active nodes based on their recommendation
        auto [farActiveNodes, leafActiveNodes, nonLeafActiveNodes] =
                partitionByDescentCriterionLockstep(activeNodes, passiveNode, descentCriterion);

        // Far-field interactions contribute to the local field
        for (auto &activeNode: farActiveNodes)
            rule(
                    activeNode.get().summary().centerOfMass(),
                    activeNode.get().summary(),
                    passiveNode.center(),
                    localField
            );

        // If this is a leaf node
        if (passiveNode.isLeaf()) {

            // Treat leaf-leaf interactions
            for (auto &activeNode: leafActiveNodes)
                Descent::none(activeNode.get(), passiveNode, rule, activeContext, passiveContext);

            // Apply forces for each contained particle
            for (auto passiveEntity: passiveNode.contents()) {

                // Switch to active-tree
                for (auto &activeNode: nonLeafActiveNodes) {
                    passiveContext.get<Acceleration>(passiveEntity) += activeTree(
                            activeNode.get(),
                            passiveContext.get<const Position>(passiveEntity),
                            descentCriterion, rule,
                            activeContext
                    );

                }

                // Include the net local force from far particles
                passiveContext.get<Acceleration>(passiveEntity) += localField.acceleration().at(
                        passiveContext.get<const Position>(passiveEntity) - passiveNode.center()
                );
            }

        } else {

            // Treat leaf active nodes using the passive-tree solver
            std::vector<Entity> activeEntititesInLeaves;
            for (auto &activeNode: leafActiveNodes)
                activeEntititesInLeaves.insert(
                        activeEntititesInLeaves.end(),
                        activeNode.get().contents().begin(), activeNode.get().contents().end()
                );
            std::span<Entity> activeEntititesInLeavesSpan{
                    activeEntititesInLeaves.begin(), activeEntititesInLeaves.end()
            };
            passiveTreeImplicitField(
                    activeEntititesInLeavesSpan,
                    passiveNode,
                    descentCriterion, rule,
                    activeContext, passiveContext
                    // _Don't_ pass down the local field, otherwise it'll be counted twice!
            );

            // For non-leaf, non-leaf interactions, we'll descend the tree (recursive case)

            // Descend other active nodes to create the active node list for the next depth
            std::vector<std::reference_wrapper<const ActiveNode>> deeperActiveNodes{};
            // todo: this is different depending on the tree type!
            deeperActiveNodes.reserve(nonLeafActiveNodes.size() * 8);
            for (auto &activeNode: nonLeafActiveNodes)
                deeperActiveNodes.insert(
                        deeperActiveNodes.end(),
                        activeNode.get().children().begin(), activeNode.get().children().end()
                );

            for (auto &child: passiveNode.children()) {
                lockstepDualTreeImplicitField<ActiveNode>(
                        deeperActiveNodes, child,
                        descentCriterion, rule,
                        activeContext, passiveContext,
                        // The local field -- including forces from far entities -- is passed down
                        {localField.acceleration().translated(child.center() - passiveNode.center())}
                );
            }
        }

    }

}

#endif //N_BODY_LOCKSTEPDUALTREEIMPLICITFIELD_H
