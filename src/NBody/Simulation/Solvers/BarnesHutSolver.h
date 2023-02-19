//
// Created by Jackson Campolattaro on 7/2/22.
//

#ifndef N_BODY_BARNESHUTSOLVER_H
#define N_BODY_BARNESHUTSOLVER_H

#include <NBody/Simulation/Solvers/ActiveTreeSolver.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>

namespace NBody {

    class BarnesHutSolver : public ActiveTreeSolver<ActiveOctree, SideLengthOverDistance> {
    public:

        using ActiveTreeSolver::ActiveTreeSolver;

        std::string id() override { return "barnes-hut"; };

        std::string name() override { return "Barnes-Hut"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

    class QuadrupoleBarnesHutSolver : public ActiveTreeSolver<
            QuadrupoleActiveOctree,
            SideLengthOverDistance
    > {
    public:

        using ActiveTreeSolver::ActiveTreeSolver;

        std::string id() override { return "barnes-hut-4p"; };

        std::string name() override { return "Barnes-Hut (Quadrupole)"; };

        int &maxDepth() { return tree().maxDepth(); }

        const int &maxDepth() const { return tree().maxDepth(); }

        int &maxLeafSize() { return tree().maxLeafSize(); }

        const int &maxLeafSize() const { return tree().maxLeafSize(); }
    };

}


#endif //N_BODY_BARNESHUTSOLVER_H
