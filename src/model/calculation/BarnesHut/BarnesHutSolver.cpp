//
// Created by jackcamp on 4/10/19.
//

#include "BarnesHutSolver.h"
#include "Octant.h"

BarnesHutSolver *BarnesHutSolver::setTheta(float theta) {
    this->theta = theta;

    return this;
}

float BarnesHutSolver::getTheta() const {
    return theta;
}

void BarnesHutSolver::solve(BodyList *bodies, PhysicsContext *phys) {


    signal_preparing_solver().emit();

    // Creating the tree
    std::unique_ptr<Octant> octree (std::make_unique<Octant>(idealTreeCenterLocation, sideLength));

    // Populating the tree
    // TODO Once the Octant is threadsafe, this will be possible to do in parallel
    //#pragma omp parallel for if(threadingEnabled)
    for (int b = 0; b < bodies->size(); ++b) {
        if (!(*bodies)[b].isPassive()) {

            // TODO Add safety checks that make sure the body is in the bounds of the tree

            octree->addBody((*bodies)[b].getPosition(), (*bodies)[b].getMass());
        }
    }

    // Getting the center of mass and the ideal location for the next tree
    octree->getCenterOfMass();
    idealTreeCenterLocation = octree->getAveragePosition();

    signal_solving().emit();

    // Doing gravitational calculations ("kick")
    #pragma omp parallel for default(none) shared(bodies, octree, phys) if(threadingEnabled)
    for (int b = 0; b < bodies->size(); ++b) {
        octree->applyPhysicsToBody(&(*bodies)[b], phys, theta);
    }

    signal_shifting_buffers().emit();

    // Updating positions ("drift")
    #pragma omp parallel for default(none) shared(bodies, octree, phys)
    for (int b = 0; b < bodies->size(); ++b) {

        (*bodies)[b].drift(phys->getT());
    }

    // Shifting buffers
    #pragma omp parallel for default(none) shared(bodies, octree, phys)
    for (int b = 0; b < bodies->size(); ++b) {

        (*bodies)[b].shiftBuffers();
    }

    signal_complete().emit();
}

