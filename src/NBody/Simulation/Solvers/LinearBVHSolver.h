//
// Created by Jackson Campolattaro on 1/11/23.
//

#ifndef N_BODY_LINEARBVHSOLVER_H
#define N_BODY_LINEARBVHSOLVER_H

#include <NBody/Simulation/Solvers/ActiveTreeSolver.h>
#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DiagonalOverDistance.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/ProjectedDiagonalOverDistance.h>

namespace NBody {

    class LinearBVHSolver : public ActiveTreeSolver<ActiveLinearBVH, Descent::DiagonalOverDistance> {
    public:

        using ActiveTreeSolver::ActiveTreeSolver;

        std::string id() override { return "linear-bvh"; };

        std::string name() override { return "Linear-BVH"; };

    };

    class QuadrupoleLinearBVHSolver : public ActiveTreeSolver<
            QuadrupoleActiveLinearBVH,
            Descent::DiagonalOverDistance
    > {
    public:

        using ActiveTreeSolver::ActiveTreeSolver;

        std::string id() override { return "linear-bvh-4p"; };

        std::string name() override { return "Linear-BVH (Quadrupole)"; };

    };
}

#endif //N_BODY_LINEARBVHSOLVER_H
