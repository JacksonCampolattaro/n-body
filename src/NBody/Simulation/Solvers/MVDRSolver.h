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

    class MVDRSolver : public DualTreeSolver<
            ActiveLinearBVH,
            QuadrupolePassiveOctree,
            Descent::DiagonalOverDistance
    > {
    public:

        using DualTreeSolver::DualTreeSolver;

        std::string id() override { return "mvdr"; };

        std::string name() override { return "Mark van de Ruit"; };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };

    template<std::size_t Order>
    class MultipoleMVDRSolver : public DualTreeSolver<
            MultipoleActiveLinearBVH<Order>,
            MultipolePassiveOctree<Order + 1>,
            Descent::DiagonalOverDistance
    > {
    public:

        using MultipoleMVDRSolver<Order>::DualTreeSolver::DualTreeSolver;
        using MultipoleMVDRSolver<Order>::DualTreeSolver::passiveTree;

        std::string id() override { return fmt::format("mvdr-{}p", std::pow(2, Order)); };

        std::string name() override { return fmt::format("Mark van de Ruit (Multipole-{})", Order); };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };

    using QuadrupoleMVDRSolver = MultipoleMVDRSolver<2>;
    using OctupoleMVDRSolver = MultipoleMVDRSolver<3>;

}

#endif //N_BODY_MVDRSOLVER_H
