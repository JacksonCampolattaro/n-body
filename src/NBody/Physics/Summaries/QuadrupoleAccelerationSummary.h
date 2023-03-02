//
// Created by Jackson Campolattaro on 2/15/23.
//

#ifndef N_BODY_QUADRUPOLEACCELERATIONSUMMARY_H
#define N_BODY_QUADRUPOLEACCELERATIONSUMMARY_H

#include <NBody/Physics/SummaryType.h>
#include <NBody/Physics/MultipoleAcceleration.h>

namespace NBody {

    class QuadrupoleAccelerationSummary {
    public:

        //using Acceleration = Physics::QuadrupoleAcceleration;
        using Acceleration = Physics::MultipoleAcceleration<2>;

    private:

        Acceleration _acceleration_{};

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

        QuadrupoleAccelerationSummary() = default;

        void summarize(const std::span<Entity> &entities, const Context &context) {}

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {}

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration_; }

        Acceleration &acceleration() { return _acceleration_; }

//        friend std::ostream &operator<<(std::ostream &out, const QuadrupoleAccelerationSummary &s) {
//            return out << "(" << s.acceleration() << ")";
//        }
    };
}

#endif //N_BODY_QUADRUPOLEACCELERATIONSUMMARY_H
