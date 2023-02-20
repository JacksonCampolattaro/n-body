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

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion>
    inline void adaptiveDualTree(
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

        switch (descentCriterion(activeNode, passiveNode)) {

            case Recommendation::Approximate:

                passiveNode.summary().acceleration() += rule(activeNode, passiveNode);
                break;

            case Recommendation::DescendActiveNode:

                if (activeNode.isLeaf()) {

                    for (auto &activeParticle: activeNode.contents())
                        Descent::passiveTree(
                                activeNode.summary().centerOfMass(), activeNode.summary().totalMass(),
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
                break;

            case Recommendation::DescendPassiveNode:

                if (passiveNode.isLeaf()) {

                    // todo: it seems like in some cases it's better to skip passive traversal entirely
                    Descent::none(activeNode, passiveNode, rule, activeContext, passiveContext);
//                    for (auto &passiveParticle: passiveNode.contents())
//                        passiveContext.get<Acceleration>(passiveParticle) += Descent::activeTree(
//                                activeNode, passiveContext.get<const Position>(passiveParticle),
//                                descentCriterion, rule,
//                                activeContext
//                        );

                } else {

                    for (auto &passiveChild: passiveNode.children())
                        Descent::adaptiveDualTree(
                                activeNode, passiveChild,
                                descentCriterion, rule,
                                activeContext, passiveContext
                        );

                }
                break;

            case Recommendation::DescendBothNodes:

                spdlog::error("Not yet implemented!");
                break;
        }

    }

}

#endif //N_BODY_ADAPTIVEDUALTREE_H
