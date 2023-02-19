//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_NONE_H
#define N_BODY_NONE_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include <NBody/Physics/Rule.h>

namespace NBody::Descent {

    // todo: This is a terrible name, I need to find a more descriptive naming scheme,
    template<NodeType ActiveNode, NodeType PassiveNode>
    inline void none(
            const ActiveNode &activeNode, PassiveNode &passiveNode,
            const Physics::Rule &rule,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, const Mass
            > &activeContext,
            const entt::basic_view<
                    entt::entity, entt::exclude_t<>,
                    const Position, Acceleration
            > &passiveContext
    ) {

        for (auto &passiveEntity: passiveNode.contents()) {
            auto &passivePosition = passiveContext.get<const Position>(passiveEntity);
            passiveContext.get<Acceleration>(passiveEntity) += std::transform_reduce(
                    activeNode.contents().begin(), activeNode.contents().end(),
                    Physics::Acceleration{}, std::plus{},
                    [&](auto activeEntity) {
                        return rule(activeContext.get<const Position>(activeEntity),
                                    activeContext.get<const Mass>(activeEntity),
                                    passivePosition);
                    }
            );
        }

    }
}

#endif //N_BODY_NONE_H
