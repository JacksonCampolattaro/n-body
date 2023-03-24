//
// Created by Jackson Campolattaro on 3/21/23.
//

#ifndef N_BODY_MULTIPOLEDUALSUMMARY_H
#define N_BODY_MULTIPOLEDUALSUMMARY_H

#include "MultipoleMassSummary.h"
#include "MultipoleAccelerationSummary.h"

namespace NBody {

    using namespace Physics;

    template<std::size_t Order>
    class MultipoleDualSummary : public MultipoleMassSummary<Order>, public MultipoleAccelerationSummary<Order> {
    public:

        using typename MultipoleMassSummary<Order>::Context;
        using MultipoleMassSummary<Order>::context;

        using MultipoleMassSummary<Order>::MultipoleMassSummary;
        using MultipoleMassSummary<Order>::summarize;
        using MultipoleMassSummary<Order>::totalMass;
        using MultipoleMassSummary<Order>::centerOfMass;

        using MultipoleAccelerationSummary<Order>::acceleration;

        friend std::ostream &operator<<(std::ostream &out, const MultipoleDualSummary<Order> &s) {
            return out << s.acceleration();
        }
    };

    using QuadrupoleDualSummary = MultipoleDualSummary<2>;
}

#endif //N_BODY_MULTIPOLEDUALSUMMARY_H
