//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_FMMSOLVER_H
#define N_BODY_FMMSOLVER_H

#include <NBody/Simulation/Solvers/DualTraversalSolver.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>

namespace NBody {

    template<std::size_t Order, RuleType Rule = Gravity>
    class MultipoleFMMSolver : public DualTraversalSolver<
            MultipoleDualOctree<Order>,
            Descent::SideLengthOverDistance,
            Rule
    > {
    public:

        using MultipoleFMMSolver<Order, Rule>::DualTraversalSolver::DualTraversalSolver;
        using MultipoleFMMSolver<Order, Rule>::DualTraversalSolver::tree;

        std::string id() override { return fmt::format("fmm-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Fast Multipole Method ({})", Multipole<Order>::name()); };

        int &maxDepth() { return tree().maxDepth(); }

        [[nodiscard]] const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        [[nodiscard]] const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    template<RuleType Rule = Gravity>
    using FMMSolver = MultipoleFMMSolver<1, Rule>;

    template<RuleType Rule = Gravity>
    using QuadrupoleFMMSolver = MultipoleFMMSolver<2, Rule>;

    template<RuleType Rule = Gravity>
    using OctupoleFMMSolver = MultipoleFMMSolver<3, Rule>;
}


#endif //N_BODY_FMMSOLVER_H
