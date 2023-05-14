//
// Created by Jackson Campolattaro on 5/5/23.
//

#ifndef N_BODY_IMPLICITFMMSOLVER_H
#define N_BODY_IMPLICITFMMSOLVER_H

#include <NBody/Simulation/Solvers/ImplicitDualTraversalSolver.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>

namespace NBody {

    template<std::size_t Order>
    class MultipoleImplicitFMMSolver : public ImplicitDualTraversalSolver<
            MultipoleImplicitDualOctree<Order>,
            Descent::SideLengthOverDistance
    > {
    public:

        using MultipoleImplicitFMMSolver<Order>::ImplicitDualTraversalSolver::ImplicitDualTraversalSolver;
        using MultipoleImplicitFMMSolver<Order>::ImplicitDualTraversalSolver::tree;

        std::string id() override {
            return fmt::format("implicit-fmm-{}p", std::pow(2, Order));
        };

        std::string name() override {
            return fmt::format("Implicit Fast Multipole Method ({})", Multipole<Order>::name());
        };

        int &maxDepth() { return tree().maxDepth(); }

        [[nodiscard]] const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        [[nodiscard]] const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    using ImplicitFMMSolver = MultipoleImplicitFMMSolver<1>;
    using QuadrupoleImplicitFMMSolver = MultipoleImplicitFMMSolver<2>;
    using OctupoleImplicitFMMSolver = MultipoleImplicitFMMSolver<3>;
}

#endif //N_BODY_IMPLICITFMMSOLVER_H
