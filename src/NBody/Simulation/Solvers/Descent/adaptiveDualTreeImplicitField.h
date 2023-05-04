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
            std::vector<std::reference_wrapper<const ActiveNode>>
    > partitionByDescentCriterion(
            std::vector<std::reference_wrapper<const ActiveNode>> &activeNodes,
            PassiveNode &passiveNode, const DescentCriterion &descentCriterion
    ) {

        auto endFarNodes = activeNodes.begin();
        auto beginLeafNodes = activeNodes.end();
        std::vector<std::reference_wrapper<const ActiveNode>> otherNodes{};

        for (auto i = activeNodes.begin(); i != beginLeafNodes; ++i) {

            auto recommendation = descentCriterion((*i).get(), passiveNode);

            if (recommendation == Recommendation::Approximate) {
                std::iter_swap(i, endFarNodes);
                ++endFarNodes;
            } else if ((*i).get().isLeaf()) {
                --beginLeafNodes;
                std::iter_swap(i, beginLeafNodes);
            } else {

                if ((recommendation & Recommendation::DescendActiveNode) == Recommendation::DoNotDescend)
                    otherNodes.emplace_back(*i);
                else
                    otherNodes.insert(
                            otherNodes.end(),
                            (*i).get().children().begin(), (*i).get().children().end()
                    );
            }

        }

        return {
                {activeNodes.begin(), endFarNodes},
                {beginLeafNodes, activeNodes.end()},
                otherNodes
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
        std::vector<std::reference_wrapper<const ActiveNode>> farActiveNodes; // todo: use a span for this!
        std::vector<std::reference_wrapper<const ActiveNode>> closeActiveNodes;
        for (auto &activeNode: activeNodes) {
            auto recommendation = descentCriterion(activeNode.get(), passiveNode);

            if (recommendation == Recommendation::Approximate)
                farActiveNodes.emplace_back(activeNode);
            else if (recommendation == Recommendation::DescendPassiveNode || activeNode.get().isLeaf())
                closeActiveNodes.emplace_back(activeNode);
            else
                closeActiveNodes.insert(
                        closeActiveNodes.end(),
                        activeNode.get().children().begin(), activeNode.get().children().end()
                );
        }

        //        auto [farActiveNodes, leafActiveNodes, closeActiveNodes] =
        //                partitionByDescentCriterion(activeNodes, passiveNode, descentCriterion);

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

            // Apply forces for each contained particle
            for (auto passiveEntity: passiveNode.contents()) {

                // Switch to active-tree
                for (auto &activeNode: closeActiveNodes) {

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

            // If it's a non-leaf node, descend the tree (recursive case)
            // The local field -- including forces from far entities -- is passed down
            for (auto &child: passiveNode.children())
                adaptiveDualTreeImplicitField(
                        closeActiveNodes, child,
                        descentCriterion, rule,
                        activeContext, passiveContext,
                        {localField.acceleration().translated(child.center() - passiveNode.center())}
                );
        }

    }

}

#endif //N_BODY_ADAPTIVEDUALTREEIMPLICITFIELD_H
