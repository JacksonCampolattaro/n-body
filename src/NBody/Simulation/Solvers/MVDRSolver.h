//
// Created by Jackson Campolattaro on 2/8/23.
//

#ifndef N_BODY_MVDRSOLVER_H
#define N_BODY_MVDRSOLVER_H

#include <NBody/Simulation/Solvers/DualTreeSolver.h>
#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DiagonalOverDistance.h>

namespace NBody {

    template<RuleType Rule = Gravity>
    class MVDRSolver : public DualTreeSolver<
            ActiveLinearBVH,
            QuadrupolePassiveOctree,
            Descent::DiagonalOverCenterDistance,
            Rule
    > {
    public:

        using DualTreeSolver<ActiveLinearBVH, QuadrupolePassiveOctree, Descent::DiagonalOverCenterDistance, Rule>::DualTreeSolver;
        using DualTreeSolver<ActiveLinearBVH, QuadrupolePassiveOctree, Descent::DiagonalOverCenterDistance, Rule>::passiveTree;

        std::string id() override { return "mvdr"; };

        std::string name() override { return "Mark van de Ruit"; };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        [[nodiscard]] const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        [[nodiscard]] const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };

    template<std::size_t Order, RuleType Rule = Gravity>
    class MultipoleMVDRSolver : public DualTreeSolver<
            MultipoleActiveLinearBVH<Order>,
            MultipolePassiveOctree<Order + 1>,
            Descent::DiagonalOverCenterDistance,
            Rule
    > {
    public:

        using MultipoleMVDRSolver<Order, Rule>::DualTreeSolver::DualTreeSolver;
        using MultipoleMVDRSolver<Order, Rule>::DualTreeSolver::passiveTree;

        std::string id() override { return fmt::format("mvdr-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Mark van de Ruit ({})", Multipole<Order>::name()); };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        [[nodiscard]] const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        [[nodiscard]] const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };

    template<RuleType Rule = Gravity>
    using QuadrupoleMVDRSolver = MultipoleMVDRSolver<2, Rule>;

    template<RuleType Rule = Gravity>
    using OctupoleMVDRSolver = MultipoleMVDRSolver<3, Rule>;

}

#endif //N_BODY_MVDRSOLVER_H
