//
// Created by Jackson Campolattaro on 5/3/23.
//

#ifndef N_BODY_PASSIVETREEIMPLICITFIELD_H
#define N_BODY_PASSIVETREEIMPLICITFIELD_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterionType.h>

namespace NBody::Descent {

    template<NodeType PassiveNode, DescentCriterionType DescentCriterion>
    inline void passiveTreeImplicitField(
            const std::span<Entity> &relevantActiveEntities,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, const Mass
            > &activeContext,
            PassiveNode &passiveNode,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, Acceleration
            > &passiveContext,
            const DescentCriterion &descentCriterion,
            const Physics::Gravity &rule,
            typename PassiveNode::Summary::ImpliedSummary localField = {}
    ) {

        // Empty nodes can be ignored
        if (passiveNode.contents().empty()) return;

        // Partition the active particle list to find the ones which can be approximated
        // Rearranging the list is only safe in single-threaded operation!
        // Each recursive root case _must_ be given its own list to rearrange.
        auto middle = std::partition(
                relevantActiveEntities.begin(), relevantActiveEntities.end(),
                [&](const Entity &e) {
                    return descentCriterion(activeContext.get<const Position>(e), passiveNode);
                }
        );
        std::span<Entity> farActiveEntities{relevantActiveEntities.begin(), middle};
        std::span<Entity> closeActiveEntities{middle, relevantActiveEntities.end()};

        // Treat the far particles using the approximated approach
        for (auto activeEntity: farActiveEntities)
            rule(activeContext.get<const Position>(activeEntity), activeContext.get<const Mass>(activeEntity),
                 passiveNode.center(), localField);


        // If this is a leaf node
        if (passiveNode.isLeaf()) {

            // Apply forces to all contained passive entities
            for (auto passiveEntity: passiveNode.contents()) {

                // Treat the close particles individually
                for (auto activeEntity: closeActiveEntities)
                    passiveContext.get<Acceleration>(passiveEntity) += rule(
                            activeContext.get<const Position>(activeEntity),
                            activeContext.get<const Mass>(activeEntity),
                            passiveContext.get<const Position>(passiveEntity)
                    );

                // Include the net local force from far particles
                passiveContext.get<Acceleration>(passiveEntity) += localField.acceleration().at(
                        passiveContext.get<const Position>(passiveEntity) - passiveNode.center()
                );

            }

        } else {

            // If it's a non-leaf node, descend the tree (recursive case)
            // The local field -- including forces from far entities -- is passed down
            for (auto &child: passiveNode.children())
                Descent::passiveTreeImplicitField(
                        closeActiveEntities, activeContext,
                        child, passiveContext,
                        descentCriterion, rule,
                        {localField.acceleration().translated(child.center() - passiveNode.center())}
                );
        }

    }

}

#endif //N_BODY_PASSIVETREEIMPLICITFIELD_H
