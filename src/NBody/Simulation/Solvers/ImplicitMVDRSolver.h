//
// Created by Jackson Campolattaro on 5/4/23.
//

#ifndef N_BODY_IMPLICITMVDRSOLVER_H
#define N_BODY_IMPLICITMVDRSOLVER_H

#include <NBody/Simulation/Solvers/ImplicitDualTreeSolver.h>
#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/DiagonalOverDistance.h>

namespace NBody {

    class ImplicitMVDRSolver : public ImplicitDualTreeSolver<
            ActiveLinearBVH,
            QuadrupoleImplicitPassiveOctree,
            Descent::DiagonalOverDistance
    > {
    public:

        using ImplicitDualTreeSolver::ImplicitDualTreeSolver;

        std::string id() override { return "implicit-mvdr"; };

        std::string name() override { return "Implicit Mark van de Ruit"; };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };

    template<std::size_t Order>
    class MultipoleImplicitMVDRSolver : public ImplicitDualTreeSolver<
            MultipoleActiveLinearBVH<Order>,
            MultipoleImplicitPassiveOctree<Order + 1>,
            Descent::DiagonalOverDistance
    > {
    public:

        using MultipoleImplicitMVDRSolver<Order>::ImplicitDualTreeSolver::ImplicitDualTreeSolver;
        using MultipoleImplicitMVDRSolver<Order>::ImplicitDualTreeSolver::passiveTree;

        std::string id() override { return fmt::format("implicit-mvdr-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Implicit Mark van de Ruit ({})", Multipole<Order>::name()); };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };

    using QuadrupoleImplicitMVDRSolver = MultipoleImplicitMVDRSolver<2>;
    using OctupoleImplicitMVDRSolver = MultipoleImplicitMVDRSolver<3>;

}

#endif //N_BODY_IMPLICITMVDRSOLVER_H
