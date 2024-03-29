//
// Created by Jackson Campolattaro on 7/2/22.
//

#ifndef N_BODY_BARNESHUTSOLVER_H
#define N_BODY_BARNESHUTSOLVER_H

#include <NBody/Simulation/Solvers/ActiveTreeSolver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>

namespace NBody {

    template<RuleType Rule>
    class BarnesHutSolver : public ActiveTreeSolver<ActiveOctree, Descent::SideLengthOverDistance, Rule> {
    public:

        using ActiveTreeSolver<ActiveOctree, Descent::SideLengthOverDistance, Rule>::ActiveTreeSolver;
        using ActiveTreeSolver<ActiveOctree, Descent::SideLengthOverDistance, Rule>::tree;

        std::string id() override { return "barnes-hut"; };

        std::string name() override { return "Barnes-Hut"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    template<std::size_t Order, RuleType Rule>
    class MultipoleBarnesHutSolver : public ActiveTreeSolver<
            MultipoleActiveOctree<Order>,
            Descent::SideLengthOverDistance,
            Rule
    > {
    public:

        using MultipoleBarnesHutSolver<Order, Rule>::ActiveTreeSolver::ActiveTreeSolver;
        using MultipoleBarnesHutSolver<Order, Rule>::ActiveTreeSolver::tree;

        // todo: these should be free functions
        std::string id() override { return fmt::format("barnes-hut-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Barnes-Hut ({})", Multipole<Order>::name()); };

        int &maxDepth() { return tree().maxDepth(); }

        [[nodiscard]] const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        [[nodiscard]] const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    template<RuleType Rule>
    using QuadrupoleBarnesHutSolver = MultipoleBarnesHutSolver<2, Rule>;

    template<RuleType Rule>
    using OctupoleBarnesHutSolver = MultipoleBarnesHutSolver<3, Rule>;

    template<RuleType Rule>
    using HexadecupoleBarnesHutSolver = MultipoleBarnesHutSolver<4, Rule>;

}


#endif //N_BODY_BARNESHUTSOLVER_H
