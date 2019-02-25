//
// Created by jackcamp on 10/19/18.
//

#include <omp.h>

#include "simulation.h"
#include "viewport.h"
#include "tracker.h"

simulation::simulation(float gravitationalConstant, float timeInterval, int power, float theta) {

    this->gravitationalConstant = gravitationalConstant;
    this->timeInterval = timeInterval;
    this->power = power;
    this->theta = theta;

    numBodies = 0;
}

void simulation::addBody(body *newBody) {

    // Adds the new body to the collection
    bodies.push_back(newBody);
    numBodies++;
}

void simulation::preCalculate() {

    // Creates the Barnes-Hut Octree
    std::unique_ptr<threadSafe_octant> octree(new threadSafe_octant(vec3(0, 0, 0), 100000));

    // Populates the Barnes-Hut Octree
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {
        #pragma omp critical
        octree->addBody(bodies[b]->getPosition(), bodies[b]->getMass());
    }

    octree->getCenterOfMass();

    timeInterval *= 0.5;

    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {
        if (!bodies[b]->isFixed()) {
            octree->applyGravityToBody(bodies[b], this);
        }
    }

    timeInterval *= 2.0;

}

void simulation::increment() {

    tracker::instance()->markFrameCompleted();

    tracker::instance()->markCalculationsStart();

    // Creates the Barnes-Hut Octree
    unique_ptr<threadSafe_octant> threadSafe_octree(new threadSafe_octant(idealTreeCenterLocation, 1000));
    tracker::instance()->markTreeCreated();


    // Populates the Octree
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {
        threadSafe_octree->addBody(bodies[b]->getPosition(), bodies[b]->getMass());
    }
    tracker::instance()->markTreeCompleted();


    // Calculates center of mass data for non-leaf nodes of the tree
    threadSafe_octree->getCenterOfMass();
    idealTreeCenterLocation = threadSafe_octree->getAveragePosition();
    tracker::instance()->markCenterMassCalculated();


    // Applies gravity to each body
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {
        if (!bodies[b]->isFixed()) {
            threadSafe_octree->applyGravityToBody(bodies[b], this);
        }
    }
    tracker::instance()->markGravityCalculated();


    // Updates each body's position
    #pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->applyVelocity(timeInterval);
    }
    tracker::instance()->markVelocityApplied();


    #pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->update();
    }
    tracker::instance()->markPositionsUpdated();

    tracker::instance()->outputStatus();




    /*// Creates the Barnes-Hut Octree
    std::unique_ptr<octant> octree(new octant(vec3(0,0,0), 1000));


    // Populates the Octree
    // Traditional Method
    for (int b = 0; b < bodies.size(); ++b) {
        octree->addBody(bodies[b]->getPosition(), bodies[b]->getMass());
    }
    tracker::instance()->markTreeCompleted();


    // Calculates center of mass data for non-leaf nodes of the tree
    octree->calculateCenterMass();
    tracker::instance()->markCenterMassCalculated();

    // Applying gravity to each body
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {
        if (!bodies[b]->isFixed()) {
            octree->applyGravity(bodies[b], theta, this);
        }
    }
    tracker::instance()->markGravityCalculated();

    // Updates each body's position
    #pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->applyVelocity(timeInterval);
    }
    tracker::instance()->markVelocityApplied();

    #pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->update();
    }
    tracker::instance()->markPositionsUpdated();

    tracker::instance()->markFrameCompleted();
    tracker::instance()->outputStatus();*/
}

void simulation::applyGravity(body *passive, vec3 activePosition, float activeMass) {

    // Calculating the directionless force of gravity
    float forceOfGravity =
            (gravitationalConstant * passive->getMass() * activeMass) /
            ((float) pow(glm::distance(passive->getPreviousPosition(), activePosition), power) + 0.0001f);

    // Giving the force direction
    glm::vec3 force = forceOfGravity * glm::normalize(activePosition - passive->getPreviousPosition());

    // Getting acceleration
    vec3 acceleration = force / passive->getMass();

    // Applying the acceleration to the body
    passive->applyAcceleration(acceleration, timeInterval);
}

void simulation::draw() {

    // Updates each body
    for (body *b : bodies) {
        b->draw();
    }
}

int simulation::getNumBodies() {
    return numBodies;
}

void simulation::orbit(body *sunBody, body *satelliteBody) {

    // Calculating the necessary velocity
    float distance = glm::distance(sunBody->getPosition(), satelliteBody->getPosition());
    float orbitalVelocity = sqrt((gravitationalConstant * sunBody->getMass() / distance));
    satelliteBody->setVelocity(orbitalVelocity * glm::normalize(satelliteBody->getVelocity()));
}

void simulation::setGravitationalConstant(float gravitationalConstant) {
    simulation::gravitationalConstant = gravitationalConstant;
}

void simulation::setTimeInterval(float timeInterval) {
    simulation::timeInterval = timeInterval;
}

void simulation::setPower(int power) {
    simulation::power = power;
}

float simulation::getTheta() const {
    return theta;
}