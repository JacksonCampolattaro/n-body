//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_LOCKSTEPDUALTREE_H
#define N_BODY_LOCKSTEPDUALTREE_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/activeTree.h>
#include <NBody/Simulation/Solvers/Descent/passiveTree.h>
#include <NBody/Simulation/Solvers/Descent/none.h>

namespace NBody::Descent {

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion>
    inline void lockstepDualTree(
            const ActiveNode &activeNode, PassiveNode &passiveNode,
            const DescentCriterion &descentCriterion, const Physics::Rule &rule,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, const Mass
            > &activeContext,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, Acceleration
            > &passiveContext
    ) {

        // If either node is empty, we have no need to calculate forces between them
        if (activeNode.contents().empty() || passiveNode.contents().empty())
            return;

        // If the nodes are far enough apart, we can use their summaries
        if (descentCriterion(activeNode, passiveNode) == Recommendation::Approximate) {

            // node-node interaction
            passiveNode.summary().acceleration() += rule(activeNode, passiveNode);

        } else if (activeNode.isLeaf() && passiveNode.isLeaf()) {

            // If both nodes are leaves & weren't far enough to summarize, then compute individual interactions
            Descent::none(activeNode, passiveNode, rule, activeContext, passiveContext);

        } else if (passiveNode.isLeaf()) {

            // If only the passive node was a leaf, continue single-tree descent
            for (auto passiveParticle: passiveNode.contents()) {
                passiveContext.get<Acceleration>(passiveParticle) +=
                        Descent::activeTree(
                                activeNode, passiveContext.get<const Position>(passiveParticle),
                                descentCriterion, rule,
                                activeContext
                        );
            }

        } else if (activeNode.isLeaf()) {

            // If only the active node was a leaf, continue single-tree descent
            for (auto activeParticle: activeNode.contents()) {
                Descent::passiveTree(
                        activeContext.template get<const Position>(activeParticle),
                        activeContext.template get<const Mass>(activeParticle),
                        passiveNode,
                        descentCriterion, rule,
                        passiveContext
                );
            }

        } else {


            // If the passive node isn't a leaf, we'll descend all its children
            std::span<PassiveNode> passiveNodesToDescend =
                    passiveNode.isLeaf() ? std::span<PassiveNode>{&passiveNode, 1}
                                         : passiveNode.children();

            // If the active node isn't a leaf, we'll descend all its children
            std::span<const ActiveNode> activeNodesToDescend =
                    activeNode.isLeaf() ? std::span<const ActiveNode>{&activeNode, 1}
                                        : activeNode.children();

            // Treat every combination of force & field node
            for (auto &childPassiveNode: passiveNodesToDescend) {
                for (const auto &childActiveNode: activeNodesToDescend) {
                    Descent::lockstepDualTree(childActiveNode, childPassiveNode,
                                              descentCriterion, rule,
                                              activeContext, passiveContext);
                }
            }
        }
    }

}

#endif //N_BODY_LOCKSTEPDUALTREE_H
