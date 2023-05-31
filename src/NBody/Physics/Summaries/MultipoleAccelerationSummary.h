//
// Created by Jackson Campolattaro on 3/20/23.
//

#ifndef N_BODY_MULTIPOLEACCELERATIONSUMMARY_H
#define N_BODY_MULTIPOLEACCELERATIONSUMMARY_H

#include "SummaryType.h"
#include <NBody/Physics/Summaries/AccelerationSummary.h>
#include <NBody/Physics/MultipoleAcceleration.h>

namespace NBody {

    using namespace Physics;

    template<std::size_t Order>
    class MultipoleAccelerationSummary {
    public:

        using Acceleration = MultipoleAcceleration<Order>;

    private:

        MultipoleAcceleration<Order> _acceleration{};

    public:

        using Context = entt::basic_view<
                entt::entity, entt::exclude_t<>,
                const Physics::Position,
                const Physics::Acceleration
        >;

        static Context context(Simulation &simulation) {
            return simulation.view<const Physics::Position, const Physics::Acceleration>();
        }

    public:

        MultipoleAccelerationSummary(const MultipoleAcceleration<Order> &acceleration = {}) :
                _acceleration(acceleration) {}

        void summarize(const std::span<Entity> &entities, const Context &context) {}

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {}

        [[nodiscard]] const MultipoleAcceleration<Order> &acceleration() const { return _acceleration; }

        MultipoleAcceleration<Order> &acceleration() { return _acceleration; }

        friend std::ostream &operator<<(std::ostream &out, const MultipoleAccelerationSummary<Order> &m) {
            return out << m.acceleration();
        }

    };

    using QuadrupoleAccelerationSummary = MultipoleAccelerationSummary<2>;
    using OctupoleAccelerationSummary = MultipoleAccelerationSummary<3>;
    using HexadecupoleAccelerationSummary = MultipoleAccelerationSummary<4>;
    using TriacontadyupoleAccelerationSummary = MultipoleAccelerationSummary<5>;

}

#endif //N_BODY_MULTIPOLEACCELERATIONSUMMARY_H
