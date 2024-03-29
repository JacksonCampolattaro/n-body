//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_NONE_H
#define N_BODY_NONE_H

#include <NBody/Simulation/Solvers/Trees/NodeType.h>
#include "NBody/Physics/Rules/Gravity.h"

namespace NBody::Descent {

    // todo: This is a terrible name, I need to find a more descriptive naming scheme,
    template<NodeType ActiveNode, RuleType Rule = Gravity>
    inline Acceleration none(
            const ActiveNode &node,
            const Position &passivePosition,
            Rule &rule,
            const ActiveView &context
    ) {
        return std::transform_reduce(
                node.contents().begin(), node.contents().end(),
                Physics::Acceleration{}, std::plus{},
                [&](auto entity) {
                    return rule(context.get<const Position>(entity),
                                context.get<const Mass>(entity),
                                passivePosition);
                }
        );
    }

    template<NodeType ActiveNode, NodeType PassiveNode, RuleType Rule = Gravity>
    inline void none(
            const ActiveNode &activeNode, PassiveNode &passiveNode,
            Rule &rule,
            const ActiveView &activeContext,
            const PassiveView &passiveContext
    ) {

        for (auto &passiveEntity: passiveNode.contents()) {

            //            auto &passivePosition = passiveContext.get<const Position>(passiveEntity);
            //            auto& passiveAcceleration = passiveContext.get<Acceleration>(passiveEntity);
            //            for (std::size_t i = 0; i < activeNode.contents().size(); ++i) {
            //                auto activeEntity = activeNode.contents()[i];
            //                passiveAcceleration += rule(
            //                        activeContext.get<const Position>(activeEntity),
            //                        activeContext.get<const Mass>(activeEntity),
            //                        passivePosition
            //                );
            //            }

            const auto &passivePosition = passiveContext.get<const Position>(passiveEntity);
            passiveContext.get<Acceleration>(passiveEntity) += std::transform_reduce(
                    activeNode.contents().begin(), activeNode.contents().end(),
                    Physics::Acceleration{}, std::plus{},
                    [&](auto &activeEntity) {
                        return rule(activeContext.get<const Position>(activeEntity),
                                    activeContext.get<const Mass>(activeEntity),
                                    passivePosition);
                    }
            );
        }

    }
}

#endif //N_BODY_NONE_H
