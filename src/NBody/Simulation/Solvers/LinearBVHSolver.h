//
// Created by Jackson Campolattaro on 1/11/23.
//

#ifndef N_BODY_LINEARBVHSOLVER_H
#define N_BODY_LINEARBVHSOLVER_H

#include <NBody/Simulation/Solvers/ActiveTreeSolver.h>
#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DiagonalOverDistance.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DiagonalOverCenterDistance.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DiagonalOverSeparation.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/MaxSideLengthOverDistance.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/ProjectedDiagonalOverDistance.h>

namespace NBody {

    template<RuleType Rule = Gravity>
    class LinearBVHSolver : public ActiveTreeSolver<ActiveLinearBVH, Descent::DiagonalOverCenterDistance, Rule> {
    public:

        using ActiveTreeSolver<ActiveLinearBVH, Descent::DiagonalOverCenterDistance, Rule>::ActiveTreeSolver;

        std::string id() override { return "linear-bvh"; };

        std::string name() override { return "Linear-BVH"; };

    };

    template<std::size_t Order, RuleType Rule = Gravity>
    class MultipoleLinearBVHSolver : public ActiveTreeSolver<
            MultipoleActiveLinearBVH<Order>,
            Descent::DiagonalOverCenterDistance,
            Rule
    > {
    public:

        using MultipoleLinearBVHSolver<Order, Rule>::ActiveTreeSolver::ActiveTreeSolver;

        std::string id() override { return fmt::format("linear-bvh-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Linear BVH ({})", Multipole<Order>::name()); };

    };

    template<RuleType Rule = Gravity>
    using QuadrupoleLinearBVHSolver = MultipoleLinearBVHSolver<2, Rule>;

    template<RuleType Rule = Gravity>
    using OctupoleLinearBVHSolver = MultipoleLinearBVHSolver<3, Rule>;

    template<RuleType Rule = Gravity>
    using HexadecupoleLinearBVHSolver = MultipoleLinearBVHSolver<4, Rule>;

}

#endif //N_BODY_LINEARBVHSOLVER_H
