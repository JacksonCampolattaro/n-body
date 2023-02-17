//
// Created by Jackson Campolattaro on 2/10/23.
//

#ifndef N_BODY_OCTREEDUALTRAVERSALSOLVER_H
#define N_BODY_OCTREEDUALTRAVERSALSOLVER_H

#include <NBody/Simulation/Solvers/DualTraversalSolver.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>

namespace NBody {

    class OctreeDualTraversalSolver : public DualTraversalSolver<
            DualOctree,
            DescentCriterion::SideLengthOverDistance
    > {
    public:

        using DualTraversalSolver::DualTraversalSolver;

        std::string id() override { return "octree-dual-traversal"; };

        std::string name() override { return "Octree Dual Traversal"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    class FMMSolver : public DualTraversalSolver<
            QuadrupoleDualOctree,
            DescentCriterion::SideLengthOverDistance
    > {
    public:

        using DualTraversalSolver::DualTraversalSolver;

        std::string id() override { return "fmm-4p"; };

        std::string name() override { return "Fast Multipole Method (Quadrupole)"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };
}

#endif //N_BODY_OCTREEDUALTRAVERSALSOLVER_H
