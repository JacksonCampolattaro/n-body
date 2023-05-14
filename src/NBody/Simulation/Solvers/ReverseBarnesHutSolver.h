//
// Created by Jackson Campolattaro on 2/22/23.
//

#ifndef N_BODY_REVERSEBARNESHUTSOLVER_H
#define N_BODY_REVERSEBARNESHUTSOLVER_H

#include <NBody/Simulation/Solvers/PassiveTreeSolver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>

namespace NBody {

    template<RuleType Rule = Gravity>
    class ReverseBarnesHutSolver : public PassiveTreeSolver<PassiveOctree, Descent::SideLengthOverDistance, Rule> {
    public:

        using PassiveTreeSolver<PassiveOctree, Descent::SideLengthOverDistance, Rule>::PassiveTreeSolver;
        using PassiveTreeSolver<PassiveOctree, Descent::SideLengthOverDistance, Rule>::tree;

        std::string id() override { return "reverse-barnes-hut"; };

        std::string name() override { return "Reverse Barnes-Hut"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    template<std::size_t Order, RuleType Rule = Gravity>
    class MultipoleReverseBarnesHutSolver : public PassiveTreeSolver<
            MultipolePassiveOctree<Order>,
            Descent::SideLengthOverDistance,
            Rule
    > {
    public:

        using MultipoleReverseBarnesHutSolver::PassiveTreeSolver::PassiveTreeSolver;
        using MultipoleReverseBarnesHutSolver::PassiveTreeSolver::tree;

        std::string id() override { return fmt::format("reverse-barnes-hut-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Reverse Barnes-Hut ({})", Multipole<Order>::name()); };

        int &maxDepth() { return tree().maxDepth(); }

        [[nodiscard]] const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        [[nodiscard]] const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    template<RuleType Rule = Gravity>
    using QuadrupoleReverseBarnesHutSolver = MultipoleReverseBarnesHutSolver<2, Rule>;

    template<RuleType Rule = Gravity>
    using OctupoleReverseBarnesHutSolver = MultipoleReverseBarnesHutSolver<3, Rule>;

    template<RuleType Rule = Gravity>
    using HexadecupoleReverseBarnesHutSolver = MultipoleReverseBarnesHutSolver<4, Rule>;

}

#endif //N_BODY_REVERSEBARNESHUTSOLVER_H
