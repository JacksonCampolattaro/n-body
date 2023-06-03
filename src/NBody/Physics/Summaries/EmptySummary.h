//
// Created by Jackson Campolattaro on 5/3/23.
//

#ifndef N_BODY_EMPTYSUMMARY_H
#define N_BODY_EMPTYSUMMARY_H

#include "SummaryType.h"

namespace NBody {

    template<typename ImpliedSummaryType>
    class EmptySummary {
    public:

        using ImpliedSummary = ImpliedSummaryType;

        using Acceleration = typename ImpliedSummary::Acceleration;

        static auto context(Simulation &simulation) {
            return simulation.view<const Physics::Position, const Physics::Acceleration>();
        }

    public:

        EmptySummary() = default;

        template<typename Context>
        void summarize(const std::span<Entity> &entities, const Context &context) {}

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {}

    };

    template<std::size_t Order>
    class MultipoleDualEmptySummary :
            public MultipoleMassSummary<Order>, public EmptySummary<MultipoleAccelerationSummary<Order + 1>> {
    public:

        using MultipoleMassSummary<Order>::context;

        using MultipoleMassSummary<Order>::MultipoleMassSummary;
        using MultipoleMassSummary<Order>::summarize;
        using MultipoleMassSummary<Order>::totalMass;
        using MultipoleMassSummary<Order>::centerOfMass;
    };

    template<>
    class MultipoleDualEmptySummary<1> :
            public CenterOfMassSummary, public EmptySummary<MultipoleAccelerationSummary<2>> {
    public:

        using CenterOfMassSummary::context;

        using CenterOfMassSummary::CenterOfMassSummary;
        using CenterOfMassSummary::summarize;
        using CenterOfMassSummary::totalMass;
        using CenterOfMassSummary::centerOfMass;
    };
}

#endif //N_BODY_EMPTYSUMMARY_H
