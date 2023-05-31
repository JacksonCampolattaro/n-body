//
// Created by Jackson Campolattaro on 5/30/23.
//

#ifndef N_BODY_IMPLICITLINEARBVHFMMSOLVER_H
#define N_BODY_IMPLICITLINEARBVHFMMSOLVER_H

#include <NBody/Simulation/Solvers/ImplicitDualTraversalSolver.h>
#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DiagonalOverDistance.h>

namespace NBody {

    template<std::size_t Order, RuleType Rule = Gravity>
    class MultipoleImplicitLinearBVHFMMSolver : public ImplicitDualTraversalSolver<
            LinearBVH<MultipoleDualEmptySummary<Order>>,
            Descent::DiagonalOverDistance,
            Rule
    > {
    public:

        using MultipoleImplicitLinearBVHFMMSolver<Order, Rule>::ImplicitDualTraversalSolver::ImplicitDualTraversalSolver;
        using MultipoleImplicitLinearBVHFMMSolver<Order, Rule>::ImplicitDualTraversalSolver::tree;

        std::string id() override {
            return fmt::format("implicit-linear-bvh-fmm-{}p", std::pow(2, Order));
        };

        std::string name() override {
            return fmt::format("Implicit Linear-BVH Fast Multipole Method ({})", Multipole<Order>::name());
        };

        int &maxDepth() { return tree().maxDepth(); }

        [[nodiscard]] const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        [[nodiscard]] const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    template<RuleType Rule = Gravity>
    using ImplicitLinearBVHFMMSolver = MultipoleImplicitLinearBVHFMMSolver<1, Rule>;

    template<RuleType Rule = Gravity>
    using QuadrupoleImplicitLinearBVHFMMSolver = MultipoleImplicitLinearBVHFMMSolver<2, Rule>;

    template<RuleType Rule = Gravity>
    using OctupoleImplicitLinearBVHFMMSolver = MultipoleImplicitLinearBVHFMMSolver<3, Rule>;
}

#endif //N_BODY_IMPLICITLINEARBVHFMMSOLVER_H
