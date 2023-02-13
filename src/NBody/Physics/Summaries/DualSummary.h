//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_DUALSUMMARY_H
#define N_BODY_DUALSUMMARY_H

#include "AccelerationSummary.h"
#include "CenterOfMassSummary.h"

namespace NBody {

    using namespace Physics;

    class DualSummary : protected CenterOfMassSummary, protected AccelerationSummary {
    public:

        using CenterOfMassSummary::Context;
        using CenterOfMassSummary::context;

        using CenterOfMassSummary::CenterOfMassSummary;
        using CenterOfMassSummary::summarize;
        using CenterOfMassSummary::totalMass;
        using CenterOfMassSummary::centerOfMass;

        using AccelerationSummary::acceleration;

    };
}

#endif //N_BODY_DUALSUMMARY_H
