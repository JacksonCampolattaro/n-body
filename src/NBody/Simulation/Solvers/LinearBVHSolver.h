//
// Created by Jackson Campolattaro on 1/11/23.
//

#ifndef N_BODY_LINEARBVHSOLVER_H
#define N_BODY_LINEARBVHSOLVER_H

#include "BarnesHutSolver.h"

#include "Trees/LinearBVH.h"
#include "Trees/DescentCriterion.h"

namespace NBody {

    class LinearBVHSolver : public BarnesHutSolverBase<LinearBVH, DescentCriterion::ProjectedDiagonalOverDistance> {
    public:

        LinearBVHSolver(Simulation &simulation, Physics::Rule &rule) :
                BarnesHutSolverBase<LinearBVH, DescentCriterion::ProjectedDiagonalOverDistance>(simulation, rule) {}

        std::string id() override { return "linear-bvh"; };

        std::string name() override { return "Linear-BVH"; };

    };
}

#endif //N_BODY_LINEARBVHSOLVER_H
