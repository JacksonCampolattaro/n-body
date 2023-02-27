//
// Created by Jackson Campolattaro on 2/22/23.
//

#ifndef N_BODY_REVERSEBARNESHUTSOLVER_H
#define N_BODY_REVERSEBARNESHUTSOLVER_H

#include <NBody/Simulation/Solvers/PassiveTreeSolver.h>
#include <NBody/Simulation/Solvers/Descent/DescentCriterion/SideLengthOverDistance.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>

namespace NBody {

    class ReverseBarnesHutSolver : public PassiveTreeSolver<QuadrupolePassiveOctree, Descent::SideLengthOverDistance> {
    public:

        using PassiveTreeSolver::PassiveTreeSolver;

        std::string id() override { return "reverse-barnes-hut"; };

        std::string name() override { return "Reverse Barnes-Hut"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

}

#endif //N_BODY_REVERSEBARNESHUTSOLVER_H
