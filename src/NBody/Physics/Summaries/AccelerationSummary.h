//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_ACCELERATIONSUMMARY_H
#define N_BODY_ACCELERATIONSUMMARY_H

#include "SummaryType.h"

namespace NBody {

    class AccelerationSummary {
    public:

        using Acceleration = Physics::Acceleration;

    private:

        Acceleration _acceleration{0.0f, 0.0f, 0.0f};

    public:

        static auto context(Simulation &simulation) {
            return simulation.view<const Physics::Position, const Physics::Acceleration>();
        }

    public:

        AccelerationSummary() = default;

        template<typename Context>
        void summarize(const std::span<Entity> &entities, const Context &context) {}

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {}

        [[nodiscard]] const Acceleration &acceleration() const { return _acceleration; }

        Acceleration &acceleration() { return _acceleration; }

    };
}

#endif //N_BODY_ACCELERATIONSUMMARY_H
