//
// Created by Jackson Campolattaro on 5/3/23.
//

#ifndef N_BODY_ADAPTIVEDUALTREEIMPLICITFIELD_H
#define N_BODY_ADAPTIVEDUALTREEIMPLICITFIELD_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/activeTree.h>
#include <NBody/Simulation/Solvers/Descent/passiveTree.h>
#include <NBody/Simulation/Solvers/Descent/none.h>

#include <NBody/Physics/Summaries/EmptySummary.h>

#include <queue>

namespace NBody::Descent {

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion>
    inline std::tuple<
            std::span<std::reference_wrapper<const ActiveNode>>,
            std::span<std::reference_wrapper<const ActiveNode>>,
            std::span<std::reference_wrapper<const ActiveNode>>
    > partitionByDescentCriterion(
            std::vector<std::reference_wrapper<const ActiveNode>> &activeNodes,
            PassiveNode &passiveNode, const DescentCriterion &descentCriterion
    ) {

        auto endFarNodes = activeNodes.begin();
        auto beginBigNodes = activeNodes.end();

        for (auto i = activeNodes.begin(); i < beginBigNodes; ++i) {
            auto &activeNode = (*i).get();
            auto recommendation = descentCriterion(activeNode, passiveNode);

            if (recommendation == Recommendation::Approximate) {

                std::iter_swap(i, endFarNodes);
                ++endFarNodes;

            } else if (recommendation != Recommendation::DescendPassiveNode &&
                       !activeNode.isLeaf()) {

                --beginBigNodes;
                std::iter_swap(i, beginBigNodes);

                // we've swapped an unknown value into position i, so we'll need to re-examine it
                i--;
            }
        }

        return {
                {activeNodes.begin(), endFarNodes},
                {endFarNodes,         beginBigNodes},
                {beginBigNodes,       activeNodes.end()}
        };
    }

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion>
    inline void adaptiveDualTreeImplicitField(
            std::vector<std::reference_wrapper<const ActiveNode>> &activeNodes,
            PassiveNode &passiveNode,
            const DescentCriterion &descentCriterion, const Physics::Rule &rule,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, const Mass
            > &activeContext,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
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
        auto [farActiveNodes, smallActiveNodes, bigActiveNodes] =
                partitionByDescentCriterion(activeNodes, passiveNode, descentCriterion);

        // Far-field interactions contribute to the local field
        for (auto &activeNode: farActiveNodes)
            rule(
                    activeNode.get().summary().centerOfMass(),
                    activeNode.get().summary(),
                    passiveNode.center(),
                    localField
            );

        //        // Leaf interactions decay to passive-tree
        //        for (auto &activeNode: leafActiveNodes) {
        //            std::vector<Entity> activeEntities{activeNode.get().contents().begin(), activeNode.get().contents().end()};
        //            std::span<Entity> activeEntitiesView{activeEntities};
        //            passiveTreeImplicitField(
        //                    activeEntitiesView, activeContext,
        //                    passiveNode, passiveContext,
        //                    descentCriterion, rule,
        //                    localField
        //            );
        //        }

        // If this is a leaf node
        if (passiveNode.isLeaf()) {

            std::span<std::reference_wrapper<const ActiveNode>> _closeActiveNodes{
                    farActiveNodes.end(),
                    activeNodes.end()
            };

            // Apply forces for each contained particle
            for (auto passiveEntity: passiveNode.contents()) {

                // Switch to active-tree
                for (auto &activeNode: _closeActiveNodes) {

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
            // If it's a non-leaf node, we'll descend the tree (recursive case)

            // Descend nearby nodes to create the active node list for the next depth
            std::vector<std::reference_wrapper<const ActiveNode>> deeperActiveNodes{};
            deeperActiveNodes.reserve(
                    smallActiveNodes.size()
                    + bigActiveNodes.size() * 2 // todo: this is different depending on the tree type!
            );
            deeperActiveNodes.insert(deeperActiveNodes.end(), smallActiveNodes.begin(), smallActiveNodes.end());
            for (auto &activeNode: bigActiveNodes) {
                deeperActiveNodes.insert(
                        deeperActiveNodes.end(),
                        activeNode.get().children().begin(), activeNode.get().children().end()
                );
            }

            // The local field -- including forces from far entities -- is passed down
            for (auto &child: passiveNode.children())
                adaptiveDualTreeImplicitField(
                        deeperActiveNodes, child,
                        descentCriterion, rule,
                        activeContext, passiveContext,
                        {localField.acceleration().translated(child.center() - passiveNode.center())}
                );
        }

    }

}

#endif //N_BODY_ADAPTIVEDUALTREEIMPLICITFIELD_H
