//
// Created by Jackson Campolattaro on 2/22/23.
//

#ifndef N_BODY_REVERSEBARNESHUTSOLVER_H
#define N_BODY_REVERSEBARNESHUTSOLVER_H

#include <NBody/Simulation/Solvers/PassiveTreeSolver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>

namespace NBody {

    class ReverseBarnesHutSolver : public PassiveTreeSolver<PassiveOctree, Descent::SideLengthOverDistance> {
    public:

        using PassiveTreeSolver::PassiveTreeSolver;

        std::string id() override { return "reverse-barnes-hut"; };

        std::string name() override { return "Reverse Barnes-Hut"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    template<std::size_t Order>
    class MultipoleReverseBarnesHutSolver : public PassiveTreeSolver<
            MultipolePassiveOctree<Order>,
            Descent::SideLengthOverDistance> {
    public:

        using MultipoleReverseBarnesHutSolver::PassiveTreeSolver::PassiveTreeSolver;
        using MultipoleReverseBarnesHutSolver::PassiveTreeSolver::tree;

        std::string id() override { return fmt::format("reverse-barnes-hut-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Reverse Barnes-Hut (Multipole-{})", Order); };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    using QuadrupoleReverseBarnesHutSolver = MultipoleReverseBarnesHutSolver<2>;
    using OctupoleReverseBarnesHutSolver = MultipoleReverseBarnesHutSolver<3>;

}

#endif //N_BODY_REVERSEBARNESHUTSOLVER_H
