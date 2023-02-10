//
// Created by Jackson Campolattaro on 1/11/23.
//

#ifndef N_BODY_LINEARBVHSOLVER_H
#define N_BODY_LINEARBVHSOLVER_H

#include <NBody/Simulation/Solvers/ActiveTreeSolver.h>
#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>

namespace NBody {

    class LinearBVHSolver : public ActiveTreeSolver<ActiveLinearBVH, DescentCriterion::DiagonalOverDistance> {
    public:

        LinearBVHSolver(Simulation &simulation, Physics::Rule &rule) :
                ActiveTreeSolver<ActiveLinearBVH, DescentCriterion::DiagonalOverDistance>(simulation, rule) {}

        std::string id() override { return "linear-bvh"; };

        std::string name() override { return "Linear-BVH"; };

    };
}

#endif //N_BODY_LINEARBVHSOLVER_H
