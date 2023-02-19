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
            PassiveOctree,
            Descent::DiagonalOverDistance
    > {
    public:

        MVDRSolver(Simulation &simulation, Physics::Rule &rule) :
                DualTreeSolver<
                        ActiveLinearBVH,
                        PassiveOctree,
                        Descent::DiagonalOverDistance
                >(simulation, rule) {
            passiveTree().maxDepth() = 16;
            passiveTree().maxLeafSize() = 64;
        }

        std::string id() override { return "mvdr"; };

        std::string name() override { return "Mark van de Ruit"; };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };

    class QuadrupoleMVDRSolver : public DualTreeSolver<
            ActiveLinearBVH,
            QuadrupolePassiveOctree,
            Descent::DiagonalOverDistance
    > {
    public:

        QuadrupoleMVDRSolver(Simulation &simulation, Physics::Rule &rule) :
                DualTreeSolver<
                        ActiveLinearBVH,
                        QuadrupolePassiveOctree,
                        Descent::DiagonalOverDistance
                >(simulation, rule) {
            passiveTree().maxDepth() = 16;
            passiveTree().maxLeafSize() = 64;
        }

        std::string id() override { return "mvdr-4p"; };

        std::string name() override { return "Mark van de Ruit (Quadrupole)"; };

        int &passiveTreeMaxDepth() { return passiveTree().maxDepth(); }

        const int &passiveTreeMaxDepth() const { return passiveTree().maxDepth(); }

        int &passiveTreeMaxLeafSize() { return passiveTree().maxLeafSize(); }

        const int &passiveTreeMaxLeafSize() const { return passiveTree().maxLeafSize(); }
    };
}

#endif //N_BODY_MVDRSOLVER_H
