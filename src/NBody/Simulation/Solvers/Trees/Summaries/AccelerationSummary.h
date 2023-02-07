//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_ACCELERATIONSUMMARY_H
#define N_BODY_ACCELERATIONSUMMARY_H

#include <NBody/Simulation/Solvers/Trees/Summary.h>

namespace NBody {

    using namespace Physics;

    class AccelerationSummary {
    private:

        Acceleration _acceleration{0.0f, 0.0f, 0.0f};

    public:

        using Context = entt::basic_group<
                entt::entity, entt::exclude_t<>,
                entt::get_t<>,
                const Position,
                const Acceleration
        >;

        static Context context(Simulation &simulation) {
            return simulation.group<const Position, const Acceleration>();
        }

    public:

        AccelerationSummary() = default;

        AccelerationSummary(const std::span<Entity> &entities, const Context &context) {}

        template<typename NodeList>
        explicit AccelerationSummary(const NodeList &childNodes) {}

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration; }

        Acceleration &acceleration() { return _acceleration; }

    };
}

#endif //N_BODY_ACCELERATIONSUMMARY_H
