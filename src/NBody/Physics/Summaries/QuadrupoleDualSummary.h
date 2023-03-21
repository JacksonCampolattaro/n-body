//
// Created by Jackson Campolattaro on 2/13/23.
//

#ifndef N_BODY_QUADRUPOLEDUALSUMMARY_H
#define N_BODY_QUADRUPOLEDUALSUMMARY_H

#include "QuadrupoleAccelerationSummary.h"

namespace NBody {

    using namespace Physics;

    class QuadrupoleDualSummary : public CenterOfMassSummary, public QuadrupoleAccelerationSummary {
    public:

        using CenterOfMassSummary::Context;
        using CenterOfMassSummary::context;

        using CenterOfMassSummary::CenterOfMassSummary;
        using CenterOfMassSummary::summarize;
        using CenterOfMassSummary::totalMass;
        using CenterOfMassSummary::centerOfMass;

//        using QuadrupoleAccelerationSummary::Acceleration;
//        using QuadrupoleAccelerationSummary::acceleration;

//        friend std::ostream &operator<<(std::ostream &out, const QuadrupoleDualSummary &s) {
//            return out << "(" << s.acceleration() << ")";
//        }
    };
}

#endif //N_BODY_QUADRUPOLEDUALSUMMARY_H
