//
// Created by Jackson Campolattaro on 2/24/23.
//

#ifndef N_BODY_BALANCEDLOCKSTEPDUALTREE_H
#define N_BODY_BALANCEDLOCKSTEPDUALTREE_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>
#include <NBody/Simulation/Solvers/Descent/lockstepDualTree.h>

namespace NBody::Descent {

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion>
    inline void balancedLockstepDualTree(
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
        if (activeNode.contents().empty() || passiveNode.contents().empty()) return;

        Recommendation recommendation = descentCriterion(activeNode, passiveNode);

        if (recommendation == Recommendation::Approximate) {

            // node-node interaction
            passiveNode.summary().acceleration() += rule(activeNode, passiveNode);

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
                    Descent::balancedLockstepDualTree(
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
                    Descent::balancedLockstepDualTree(
                            activeNode, passiveChild,
                            descentCriterion, rule,
                            activeContext, passiveContext
                    );

            }

        } else if (recommendation == Recommendation::DescendBothNodes) {

            // Once the node sizes are close enough, switch to lockstep
            lockstepDualTree(activeNode, passiveNode, descentCriterion, rule, activeContext, passiveContext);
        }
    }

}

#endif //N_BODY_BALANCEDLOCKSTEPDUALTREE_H
