//
// Created by Jackson Campolattaro on 2/8/23.
//

#ifndef N_BODY_MVDRSOLVER_H
#define N_BODY_MVDRSOLVER_H

#include <NBody/Simulation/Solvers/DualTreeSolver.h>
#include <NBody/Simulation/Solvers/Trees/LinearBVH.h>
#include <NBody/Simulation/Solvers/Trees/Octree.h>
#include <NBody/Simulation/Solvers/Trees/DescentCriterion.h>

namespace NBody {

    class MVDRSolver : public DualTreeSolver<ActiveLinearBVH, PassiveOctree, DescentCriterion::DiagonalOverDistance> {
    public:

        MVDRSolver(Simulation &simulation, Physics::Rule &rule) :
                DualTreeSolver<
                        ActiveLinearBVH,
                        PassiveOctree,
                        DescentCriterion::DiagonalOverDistance
                >(simulation, rule) {
            passiveTree().maxDepth() = 32;
            passiveTree().maxLeafSize() = 16;
        }

        std::string id() override { return "mvdr"; };

        std::string name() override { return "Mark van de Ruit"; };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };
}

#endif //N_BODY_MVDRSOLVER_H
