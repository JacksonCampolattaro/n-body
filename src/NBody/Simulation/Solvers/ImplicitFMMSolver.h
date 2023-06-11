//
// Created by Jackson Campolattaro on 5/5/23.
//

#ifndef N_BODY_IMPLICITFMMSOLVER_H
#define N_BODY_IMPLICITFMMSOLVER_H

#include <NBody/Simulation/Solvers/ImplicitDualTraversalSolver.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>

namespace NBody {

    template<std::size_t Order, RuleType Rule = Gravity>
    class MultipoleImplicitFMMSolver : public ImplicitDualTraversalSolver<
            MultipoleImplicitDualOctree<Order>,
            Descent::SideLengthOverDistance,
            Rule
    > {
    public:

        using MultipoleImplicitFMMSolver<Order, Rule>::ImplicitDualTraversalSolver::ImplicitDualTraversalSolver;
        using MultipoleImplicitFMMSolver<Order, Rule>::ImplicitDualTraversalSolver::tree;

        std::string id() override {
            return fmt::format("implicit-fmm-{}p", std::pow(2, Order));
        };

        std::string name() override {
            return fmt::format("Implicit Fast Multipole Method ({})", Multipole<Order>::name());
        };
    };

    template<RuleType Rule = Gravity>
    using ImplicitFMMSolver = MultipoleImplicitFMMSolver<1, Rule>;

    template<RuleType Rule = Gravity>
    using QuadrupoleImplicitFMMSolver = MultipoleImplicitFMMSolver<2, Rule>;

    template<RuleType Rule = Gravity>
    using OctupoleImplicitFMMSolver = MultipoleImplicitFMMSolver<3, Rule>;

    template<RuleType Rule = Gravity>
    using HexadecupoleImplicitFMMSolver = MultipoleImplicitFMMSolver<4, Rule>;
}

#endif //N_BODY_IMPLICITFMMSOLVER_H
