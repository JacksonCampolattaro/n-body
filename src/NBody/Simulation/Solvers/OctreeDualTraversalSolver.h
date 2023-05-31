//
// Created by Jackson Campolattaro on 2/10/23.
//

#ifndef N_BODY_OCTREEDUALTRAVERSALSOLVER_H
#define N_BODY_OCTREEDUALTRAVERSALSOLVER_H

#include <NBody/Simulation/Solvers/DualTraversalSolver.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>

namespace NBody {

    template<RuleType Rule = Gravity>
    class OctreeDualTraversalSolver : public DualTraversalSolver<
            DualOctree,
            Descent::SideLengthOverDistance,
            Rule
    > {
    public:

        using DualTraversalSolver<DualOctree, Descent::SideLengthOverDistance, Rule>::DualTraversalSolver;
        using DualTraversalSolver<DualOctree, Descent::SideLengthOverDistance, Rule>::tree;

        std::string id() override { return "octree-dual-traversal"; };

        std::string name() override { return "Octree Dual Traversal"; };

        int &maxDepth() { return tree().maxDepth(); }

        [[nodiscard]] const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        [[nodiscard]] const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

}

#endif //N_BODY_OCTREEDUALTRAVERSALSOLVER_H
