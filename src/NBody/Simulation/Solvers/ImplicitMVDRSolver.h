//
// Created by Jackson Campolattaro on 5/4/23.
//

#ifndef N_BODY_IMPLICITMVDRSOLVER_H
#define N_BODY_IMPLICITMVDRSOLVER_H

#include <NBody/Simulation/Solvers/ImplicitDualTreeSolver.h>
#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DiagonalOverDistance.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DiagonalOverSeparation.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DualThetaDiagonalOverDistance.h>

namespace NBody {

    template<RuleType Rule = Gravity>
    class ImplicitMVDRSolver : public ImplicitDualTreeSolver<
            ActiveLinearBVH,
            QuadrupoleImplicitPassiveOctree,
            Descent::DiagonalOverDistance,
            Rule
    > {
    public:

        using ImplicitDualTreeSolver<ActiveLinearBVH, QuadrupoleImplicitPassiveOctree, Descent::DiagonalOverDistance, Rule>::ImplicitDualTreeSolver;
        using ImplicitDualTreeSolver<ActiveLinearBVH, QuadrupoleImplicitPassiveOctree, Descent::DiagonalOverDistance, Rule>::passiveTree;
        using ImplicitDualTreeSolver<ActiveLinearBVH, QuadrupoleImplicitPassiveOctree, Descent::DiagonalOverDistance, Rule>::activeTree;

        std::string id() override { return "implicit-mvdr"; };

        std::string name() override { return "Implicit Mark van de Ruit"; };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        [[nodiscard]] const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        [[nodiscard]] const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };

    template<std::size_t Order, RuleType Rule = Gravity>
    class MultipoleImplicitMVDRSolver : public ImplicitDualTreeSolver<
            MultipoleActiveLinearBVH<Order>,
            MultipoleImplicitPassiveOctree<Order + 1>,
            Descent::DiagonalOverDistance,
            Rule
    > {
    public:

        using MultipoleImplicitMVDRSolver<Order, Rule>::ImplicitDualTreeSolver::ImplicitDualTreeSolver;
        using MultipoleImplicitMVDRSolver<Order, Rule>::ImplicitDualTreeSolver::passiveTree;

        std::string id() override { return fmt::format("implicit-mvdr-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Implicit Mark van de Ruit ({})", Multipole<Order>::name()); };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        [[nodiscard]] const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        [[nodiscard]] const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };

    template<RuleType Rule = Gravity>
    using QuadrupoleImplicitMVDRSolver = MultipoleImplicitMVDRSolver<2, Rule>;

    template<RuleType Rule = Gravity>
    using OctupoleImplicitMVDRSolver = MultipoleImplicitMVDRSolver<3, Rule>;

}

#endif //N_BODY_IMPLICITMVDRSOLVER_H
