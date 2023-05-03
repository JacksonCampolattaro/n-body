//
// Created by Jackson Campolattaro on 5/3/23.
//

#ifndef N_BODY_EMPTYSUMMARY_H
#define N_BODY_EMPTYSUMMARY_H

#include "NBody/Physics/SummaryType.h"

namespace NBody {

    template<typename ImpliedSummaryType>
    class EmptySummary {
    public:

        using ImpliedSummary = ImpliedSummaryType;

        using Acceleration = typename ImpliedSummary::Acceleration;

        using Context = entt::basic_view<
                entt::entity, entt::exclude_t<>,
                const Physics::Position,
                const Physics::Acceleration
        >;

        static Context context(Simulation &simulation) {
            return simulation.view<const Physics::Position, const Physics::Acceleration>();
        }

    public:

        EmptySummary() = default;

        void summarize(const std::span<Entity> &entities, const Context &context) {}

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {}

    };

}

#endif //N_BODY_EMPTYSUMMARY_H
