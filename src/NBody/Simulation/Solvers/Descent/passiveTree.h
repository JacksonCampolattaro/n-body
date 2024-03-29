//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_PASSIVETREE_H
#define N_BODY_PASSIVETREE_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

namespace NBody::Descent {

    template<NodeType PassiveNode, DescentCriterionType DescentCriterion, RuleType Rule = Gravity>
    inline void passiveTree(
            const Position &activePosition,
            const Mass &activeMass,
            PassiveNode &passiveNode,
            const DescentCriterion &descentCriterion,
            Rule &rule,
            const PassiveView &context
    ) {

        // Empty nodes can be ignored
        if (passiveNode.contents().empty()) return;

        if (descentCriterion(activePosition, passiveNode)) {

            // If an approximation is allowed, compute particle-node interaction
            rule(activePosition, activeMass, passiveNode);

        } else {

            // Otherwise, the passive node can't be summarized
            if (passiveNode.isLeaf()) {

                // If this is a leaf passive node, interact with all particles contained
                for (auto entity: passiveNode.contents())
                    context.get<Acceleration>(entity) += rule(
                            activePosition, activeMass,
                            context.get<const Position>(entity)
                    );

            } else {

                // If it's a non-leaf node, descend the tree (recursive case)
                for (auto &child: passiveNode.children())
                    Descent::passiveTree(
                            activePosition, activeMass, child,
                            descentCriterion, rule,
                            context
                    );

            }
        }
    }

}

#endif //N_BODY_PASSIVETREE_H
