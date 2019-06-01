//
// Created by jackcamp on 5/30/19.
//

#include <iostream>
#include "refactoredBarnesHutSolver.h"



void refactoredBarnesHutSolver::build(std::vector<Body *> bodies, PhysicsContext *physicsContext) {
    refactoredSolver::build(bodies, physicsContext);

    octree = generateOctree();


    std::cout << octree->getNumBodies() << std::endl;

    // Populating the tree
    /*Only active bodies are added*/
    // TODO make this thread safe
    for (int b = 0; b < activeBodies.size(); ++b) {
        octree->addBody(activeBodies[b]->getPosition(), activeBodies[b]->getMass());
    }

    std::cout << octree->getNumBodies() << std::endl;

    // Center of mass must be calculated before solving starts
    octree->getCenterOfMass();

    // This finds the most efficient center location for the next cycle's tree
    idealTreeCenterLocation = octree->getAveragePosition();
}


void refactoredBarnesHutSolver::kick(Body *subjectBody) {

    // Uses the tree to apply gravity to a specific body
    octree->applyPhysicsToBody(subjectBody, physicsContext, theta);
}

refactoredBarnesHutSolver *refactoredBarnesHutSolver::setTheta(float theta) {
    this->theta = theta;

    return this;

}

std::unique_ptr<Octant> refactoredBarnesHutSolver::generateOctree() {

    return std::make_unique<Octant>(idealTreeCenterLocation, sideLength);
}
