//
// Created by Jackson Campolattaro on 2/13/23.
//

#ifndef N_BODY_QUADRUPOLEDUALSUMMARY_H
#define N_BODY_QUADRUPOLEDUALSUMMARY_H

#include "QuadrupoleAccelerationSummary.h"
#include "QuadrupoleMassSummary.h"

namespace NBody {

    using namespace Physics;

    class QuadrupoleDualSummary : protected QuadrupoleMassSummary, protected QuadrupoleAccelerationSummary {
    public:

        using QuadrupoleMassSummary::Context;
        using QuadrupoleMassSummary::context;

        using QuadrupoleMassSummary::QuadrupoleMassSummary;
        using QuadrupoleMassSummary::summarize;
        using QuadrupoleMassSummary::totalMass;
        using QuadrupoleMassSummary::centerOfMass;
        using QuadrupoleMassSummary::moment;

        using QuadrupoleAccelerationSummary::acceleration;

    };
}

#endif //N_BODY_QUADRUPOLEDUALSUMMARY_H
