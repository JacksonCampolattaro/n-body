//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_ADAPTIVEDUALTREE_H
#define N_BODY_ADAPTIVEDUALTREE_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/activeTree.h>
#include <NBody/Simulation/Solvers/Descent/passiveTree.h>
#include <NBody/Simulation/Solvers/Descent/none.h>

namespace NBody::Descent {

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion, RuleType Rule = Gravity>
    inline void adaptiveDualTree(
            const ActiveNode &activeNode, PassiveNode &passiveNode,
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

        // If either node is empty, we have no need to calculate forces between them
        if (activeNode.contents().empty() || passiveNode.contents().empty())
            return;

        Recommendation recommendation = descentCriterion(activeNode, passiveNode);

        if (recommendation == Recommendation::Approximate) {

            rule(activeNode, passiveNode);

        } else if (recommendation == Recommendation::DescendActiveNode) {

            if (activeNode.isLeaf()) {

                for (auto &activeParticle: activeNode.contents())
                    Descent::passiveTree(
                            activeContext.template get<const Position>(activeParticle),
                            activeContext.template get<const Mass>(activeParticle),
                            passiveNode,
                            descentCriterion, rule,
                            passiveContext
                    );

            } else {

                for (auto &activeChild: activeNode.children())
                    Descent::adaptiveDualTree(
                            activeChild, passiveNode,
                            descentCriterion, rule,
                            activeContext, passiveContext
                    );

            }

        } else if (recommendation == Recommendation::DescendPassiveNode) {

            if (passiveNode.isLeaf()) {

                for (auto &passiveParticle: passiveNode.contents())
                    passiveContext.get<Acceleration>(passiveParticle) += Descent::activeTree(
                            activeNode, passiveContext.get<const Position>(passiveParticle),
                            descentCriterion, rule,
                            activeContext
                    );

            } else {

                for (auto &passiveChild: passiveNode.children())
                    Descent::adaptiveDualTree(
                            activeNode, passiveChild,
                            descentCriterion, rule,
                            activeContext, passiveContext
                    );

            }

        } else if (recommendation == Recommendation::DescendBothNodes) {

            if (passiveNode.isLeaf()) {

                for (auto &passiveParticle: passiveNode.contents())
                    passiveContext.get<Acceleration>(passiveParticle) += Descent::activeTree(
                            activeNode, passiveContext.get<const Position>(passiveParticle),
                            descentCriterion, rule,
                            activeContext
                    );

            } else if (activeNode.isLeaf()) {

                for (auto &activeParticle: activeNode.contents())
                    Descent::passiveTree(
                            activeContext.template get<const Position>(activeParticle),
                            activeContext.template get<const Mass>(activeParticle),
                            passiveNode,
                            descentCriterion, rule,
                            passiveContext
                    );

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
                        Descent::adaptiveDualTree(childActiveNode, childPassiveNode,
                                                  descentCriterion, rule,
                                                  activeContext, passiveContext);
                    }
                }
            }

        }

    }

}

#endif //N_BODY_ADAPTIVEDUALTREE_H
