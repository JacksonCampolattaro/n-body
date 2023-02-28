//
// Created by Jackson Campolattaro on 2/19/23.
//

#ifndef N_BODY_FMMSOLVER_H
#define N_BODY_FMMSOLVER_H

#include <NBody/Simulation/Solvers/DualTraversalSolver.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>

namespace NBody {

    class FMMSolver : public DualTraversalSolver<
            QuadrupoleDualOctree,
            Descent::SideLengthOverDistance
    > {
    public:

        using DualTraversalSolver::DualTraversalSolver;

        std::string id() override { return "fmm-4p"; };

        std::string name() override { return "Fast SymmetricMatrix3 Method (Quadrupole)"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };
}


#endif //N_BODY_FMMSOLVER_H
