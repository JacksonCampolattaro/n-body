//
// Created by jackcamp on 4/10/19.
//

#include "BarnesHutSolver.h"
#include "Octant.h"

void BarnesHutSolver::solve(std::vector<Body *> bodies, PhysicsContext *phys) {

    // Creating the tree
    std::unique_ptr<Octant> octree (std::make_unique<Octant>(idealTreeCenterLocation, sideLength));

    // Populating the tree
    // TODO Once the Octant is threadsafe, this will be possible to do in parallel
    //#pragma omp parallel for if(threadingEnabled)
    for (int b = 0; b < bodies.size(); ++b) {
        if (!bodies[b]->isPassive()) {
            octree->addBody(bodies[b]->getPosition(), bodies[b]->getMass());
        }
    }

    // Getting the center of mass and the ideal location for the next tree
    octree->getCenterOfMass();
    idealTreeCenterLocation = octree->getAveragePosition();

    // Doing gravitational calculations
    #pragma omp parallel for if(threadingEnabled)
    for (int b = 0; b < bodies.size(); ++b) {
        octree->applyPhysicsToBody(bodies[b], phys, theta);
    }
}

BarnesHutSolver *BarnesHutSolver::setTheta(float theta) {
    this->theta = theta;

    return this;
}

float BarnesHutSolver::getTheta() const {
    return theta;
}

