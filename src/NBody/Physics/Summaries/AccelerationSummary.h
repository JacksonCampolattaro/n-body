//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_ACCELERATIONSUMMARY_H
#define N_BODY_ACCELERATIONSUMMARY_H

#include "NBody/Physics/SummaryType.h"

namespace NBody {

    using namespace Physics;

    class AccelerationSummary {
    private:

        Acceleration _acceleration{0.0f, 0.0f, 0.0f};

    public:

        using Context = entt::basic_view<
                entt::entity, entt::exclude_t<>,
                const NBody::Physics::Position,
                const NBody::Physics::Acceleration
        >;

        static Context context(Simulation &simulation) {
            return simulation.view<const Position, const Acceleration>();
        }

    public:

        AccelerationSummary() = default;

        void summarize(const std::span<Entity> &entities, const Context &context) {}

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {}

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration; }

        Acceleration &acceleration() { return _acceleration; }

    };
}

#endif //N_BODY_ACCELERATIONSUMMARY_H
