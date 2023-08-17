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

    template<NodeType ActiveNode, NodeType PassiveNode, DescentCriterionType DescentCriterion, RuleType Rule = Gravity>
    inline void lockstepDualTree(
            const ActiveNode &activeNode, PassiveNode &passiveNode,
            const DescentCriterion &descentCriterion, Rule &rule,
            const ActiveView &activeContext,
            const PassiveView &passiveContext
    ) {

        // If either node is empty, we have no need to calculate forces between them
        if (activeNode.contents().empty() || passiveNode.contents().empty())
            return;

        // If the nodes are far enough apart, we can use their summaries
        if (descentCriterion(activeNode, passiveNode) == Recommendation::Approximate) {

            // node-node interaction
            rule(activeNode, passiveNode);

        } else if (passiveNode.isLeaf()) {

            // If only the passive node was a leaf, continue single-tree descent
            for (auto &passiveParticle: passiveNode.contents()) {
                passiveContext.get<Acceleration>(passiveParticle) += Descent::activeTree(
                        activeNode, passiveContext.get<const Position>(passiveParticle),
                        descentCriterion, rule,
                        activeContext
                );
            }

        } else if (activeNode.isLeaf()) {

            // If only the active node was a leaf, continue single-tree descent
            for (auto &activeParticle: activeNode.contents()) {
                Descent::passiveTree(
                        activeContext.template get<const Position>(activeParticle),
                        activeContext.template get<const Mass>(activeParticle),
                        passiveNode,
                        descentCriterion, rule,
                        passiveContext
                );
            }

        } else {


            // Treat every combination of force & field child
            for (auto &childPassiveNode: passiveNode.children()) {
                for (const auto &childActiveNode: activeNode.children()) {
                    Descent::lockstepDualTree(childActiveNode, childPassiveNode,
                                              descentCriterion, rule,
                                              activeContext, passiveContext);
                }
            }
        }
    }

}

#endif //N_BODY_LOCKSTEPDUALTREE_H
