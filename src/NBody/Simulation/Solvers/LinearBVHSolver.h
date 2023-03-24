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

    template<std::size_t Order>
    class MultipoleLinearBVHSolver : public ActiveTreeSolver<
            MultipoleActiveLinearBVH<Order>, // fixme: why doesn't this make a difference to performance?
            Descent::DiagonalOverDistance
    > {
    public:

        using MultipoleLinearBVHSolver<Order>::ActiveTreeSolver::ActiveTreeSolver;

        std::string id() override { return fmt::format("linear-bvh-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Linear BVH (Multipole-{})", Order); };

    };

    using QuadrupoleLinearBVHSolver = MultipoleLinearBVHSolver<2>;
    using OctupoleLinearBVHSolver = MultipoleLinearBVHSolver<3>;

}

#endif //N_BODY_LINEARBVHSOLVER_H
