//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_ACTIVETREE_H
#define N_BODY_ACTIVETREE_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

namespace NBody::Descent {

    template<NodeType ActiveNode, DescentCriterionType DescentCriterion>
    inline Acceleration activeTree(
            const ActiveNode &node,
            const Position &passivePosition,
            const DescentCriterion &descentCriterion,
            const Physics::Gravity &rule,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, const Mass
            > &context
    ) {

        // Empty nodes can be ignored
        if (node.contents().empty()) return Physics::Acceleration{};

        if (descentCriterion(node, passivePosition)) {
            return rule(node, passivePosition);
        } else {

            // Otherwise, the node can't be summarized
            if (node.isLeaf()) {

                // If this is a leaf node, interact with all particles contained
                return std::transform_reduce(
                        node.contents().begin(), node.contents().end(),
                        Physics::Acceleration{}, std::plus{},
                        [&](auto entity) {
                            return rule(context.get<const Position>(entity),
                                         context.get<const Mass>(entity),
                                         passivePosition);
                        }
                );

            } else {

                // If it's a non-leaf node, descend the tree (recursive case)
                return std::transform_reduce(
                        node.children().begin(), node.children().end(),
                        Physics::Acceleration{}, std::plus{},
                        [&](const auto &child) {
                            return activeTree(
                                    child, passivePosition,
                                    descentCriterion, rule,
                                    context
                            );
                        }
                );
            }
        }
    }

}

#endif //N_BODY_ACTIVETREE_H
