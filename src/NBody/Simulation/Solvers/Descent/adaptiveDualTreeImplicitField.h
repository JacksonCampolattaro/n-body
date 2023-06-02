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
            std::span<std::reference_wrapper<const ActiveNode>>,
            std::span<std::reference_wrapper<const ActiveNode>>
    > sortByDescentCriterion(
            std::span<std::reference_wrapper<const ActiveNode>> activeNodes,
            PassiveNode &passiveNode, const DescentCriterion &descentCriterion
    ) {

        auto endFarNodes = activeNodes.begin();
        auto beginBigNodes = activeNodes.end();
        auto beginLeafNodes = activeNodes.end();

        for (auto i = activeNodes.begin(); i < beginBigNodes; ++i) {
            auto &activeNode = (*i).get();
            auto recommendation = descentCriterion(activeNode, passiveNode);

            if (recommendation == Recommendation::Approximate) {

                std::iter_swap(i, endFarNodes);
                ++endFarNodes;

            } else if (activeNode.isLeaf()) {

                --beginBigNodes;
                --beginLeafNodes;
                std::iter_swap(i, beginLeafNodes);
                if (beginLeafNodes != beginBigNodes)
                    std::iter_swap(i, beginBigNodes);

                // we've swapped an unknown value into position i, so we'll need to re-examine it
                i--;

            } else if (recommendation != Recommendation::DescendPassiveNode) {

                --beginBigNodes;
                std::iter_swap(i, beginBigNodes);

                // we've swapped an unknown value into position i, so we'll need to re-examine it
                i--;
            }
        }

        return {
                {activeNodes.begin(), endFarNodes},
                {endFarNodes,         beginBigNodes},
                {beginBigNodes,       beginLeafNodes},
                {beginLeafNodes,      activeNodes.end()},
        };
    }

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion>
    inline std::tuple<
            std::span<std::reference_wrapper<const ActiveNode>>,
            std::span<std::reference_wrapper<const ActiveNode>>,
            std::span<std::reference_wrapper<const ActiveNode>>
    > partitionByDescentCriterion(
            std::span<std::reference_wrapper<const ActiveNode>> activeNodes,
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

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion, RuleType Rule = Gravity>
    inline void adaptiveDualTreeImplicitField(
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
        auto [farActiveNodes, smallActiveNodes, bigActiveNodes, leafActiveNodes] =
                sortByDescentCriterion(activeNodes, passiveNode, descentCriterion);

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

            std::span<std::reference_wrapper<const ActiveNode>> closeActiveNodes{
                    smallActiveNodes.begin(),
                    bigActiveNodes.end()
            };

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

            // If it's a non-leaf node, we'll descend the tree (recursive case)

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

            // Descend nearby nodes to create the active node list for the next depth
            // Small nodes are descended once, too-large nodes are doubly descended
            std::vector<std::reference_wrapper<const ActiveNode>> deeperActiveNodes{};
            deeperActiveNodes.reserve(
                    //smallActiveNodes.size() +
                    bigActiveNodes.size() * 2 // todo: this is different depending on the tree type!
            );
            //deeperActiveNodes.insert(deeperActiveNodes.end(), smallActiveNodes.begin(), smallActiveNodes.end());
            for (auto &activeNode: bigActiveNodes) {
                deeperActiveNodes.insert(
                        deeperActiveNodes.end(),
                        activeNode.get().children().begin(), activeNode.get().children().end()
                );
            }

            for (auto &child: passiveNode.children()) {
                adaptiveDualTreeImplicitField<ActiveNode>(
                        smallActiveNodes, child,
                        descentCriterion, rule,
                        activeContext, passiveContext,
                        // The local field -- including forces from far entities -- is passed down
                        {localField.acceleration().translated(child.center() - passiveNode.center())}
                );
            }

            adaptiveDualTreeImplicitField<ActiveNode>(
                    deeperActiveNodes, passiveNode,
                    descentCriterion, rule,
                    activeContext, passiveContext
            );
        }

    }

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion, RuleType Rule = Gravity>
    inline void adaptiveDualTreeImplicitField(
            const ActiveNode &startingActiveNode, PassiveNode &passiveNode,
            const DescentCriterion &descentCriterion, Rule &rule,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, const Mass
            > &activeContext,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, Acceleration
            > &passiveContext
    ) {


        std::queue<std::reference_wrapper<const ActiveNode>> activeNodesQueue{{startingActiveNode}};
        std::vector<std::reference_wrapper<const ActiveNode>> equalSizeActiveNodes{};

        while (!activeNodesQueue.empty()) {
            auto &activeNode = activeNodesQueue.front().get();

            Recommendation recommendation = descentCriterion(activeNode, passiveNode);
            activeNodesQueue.pop();

            if (recommendation != Recommendation::DescendActiveNode || activeNode.isLeaf()) {
                equalSizeActiveNodes.emplace_back(activeNode);
            } else {
                for (auto &child: activeNode.children())
                    activeNodesQueue.push(child);
            }
        }

        adaptiveDualTreeImplicitField<ActiveNode>(
                equalSizeActiveNodes, passiveNode,
                descentCriterion, rule,
                activeContext, passiveContext
        );

    }
}

#endif //N_BODY_ADAPTIVEDUALTREEIMPLICITFIELD_H
