//
// Created by Jackson Campolattaro on 5/3/23.
//

#ifndef N_BODY_IMPLICITREVERSEBARNESHUTSOLVER_H
#define N_BODY_IMPLICITREVERSEBARNESHUTSOLVER_H

#include <NBody/Simulation/Solvers/ImplicitPassiveTreeSolver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>

namespace NBody {

    template<RuleType Rule = Gravity>
    class ImplicitReverseBarnesHutSolver : public ImplicitPassiveTreeSolver<
            ImplicitPassiveOctree,
            Descent::SideLengthOverDistance,
            Rule
    > {
    public:

        using ImplicitPassiveTreeSolver<ImplicitPassiveOctree, Descent::SideLengthOverDistance, Rule>::ImplicitPassiveTreeSolver;
        using ImplicitPassiveTreeSolver<ImplicitPassiveOctree, Descent::SideLengthOverDistance, Rule>::tree;

        std::string id() override { return "implicit-reverse-barnes-hut"; };

        std::string name() override { return "Implicit Reverse Barnes-Hut"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    template<std::size_t Order, RuleType Rule = Gravity>
    class MultipoleImplicitReverseBarnesHutSolver : public ImplicitPassiveTreeSolver<
            MultipoleImplicitPassiveOctree<Order>,
            Descent::SideLengthOverDistance,
            Rule
    > {
    public:

        using MultipoleImplicitReverseBarnesHutSolver::ImplicitPassiveTreeSolver::ImplicitPassiveTreeSolver;
        using MultipoleImplicitReverseBarnesHutSolver::ImplicitPassiveTreeSolver::tree;

        std::string id() override {
            return fmt::format("implicit-reverse-barnes-hut-{}p", std::pow(2, Order));
        };

        std::string name() override {
            return fmt::format("Implicit Reverse Barnes-Hut ({})", Multipole<Order>::name());
        };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    template<RuleType Rule = Gravity>
    using QuadrupoleImplicitReverseBarnesHutSolver = MultipoleImplicitReverseBarnesHutSolver<2, Rule>;

    template<RuleType Rule = Gravity>
    using OctupoleImplicitReverseBarnesHutSolver = MultipoleImplicitReverseBarnesHutSolver<3, Rule>;

    template<RuleType Rule = Gravity>
    using HexadecupoleImplicitReverseBarnesHutSolver = MultipoleImplicitReverseBarnesHutSolver<4, Rule>;

    template<RuleType Rule = Gravity>
    using TriacontadyupoleImplicitReverseBarnesHutSolver = MultipoleImplicitReverseBarnesHutSolver<5, Rule>;

}

#endif //N_BODY_IMPLICITREVERSEBARNESHUTSOLVER_H
