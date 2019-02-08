//
// Created by jackcamp on 10/19/18.
//

#include <omp.h>
#include "simulation.h"
#include "viewport.h"

simulation::simulation(float gravitationalConstant, float timeInterval, int power) {

    this->gravitationalConstant = gravitationalConstant;
    this->timeInterval = timeInterval;
    this->power = power;
}

void simulation::addBody(body *newBody) {

    // Adds the new body to the collection
    bodies.push_back(newBody);
}

void simulation::increment() {

    cout << endl << "Starting physics increment, all times in clock ticks" << endl;
    cout << std::to_string(bodies.size()) << " bodies in simulation." << endl;
    clock_t startTime;
    clock_t endTime;

    // Creates the Barnes-Hut Octree
    startTime = clock();
    std::unique_ptr<octant> octree(new octant(vec3(0, 0, 0), 100000));
    endTime = clock();
    cout << "Creating the root octant: " << std::to_string(endTime - startTime) << endl;

    // Populates the Barnes-Hut Octree
    startTime = clock();
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {
        #pragma omp critical
        octree->addBody(bodies[b]->getPosition(), bodies[b]->getMass());
    }
    //octree->addBodies(bodies);
    endTime = clock();
    cout << "Populating the octree: " << std::to_string(endTime - startTime) << endl;

    // Calculates center of mass data for non-leaf nodes of the tree
    startTime = clock();
    octree->calculateCenterMass();
    endTime = clock();
    cout << "Generating center of mass data: " << std::to_string(endTime - startTime) << endl;

    // Applying gravity to each body
    startTime = clock();
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {
        if (!bodies[b]->isFixed()) {
            octree->applyGravity(bodies[b], 0.8, this);
        }
    }
    endTime = clock();
    cout << "Applying gravity: " << std::to_string(endTime - startTime) << endl;

    // Updates each body's position
    startTime = clock();
    #pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->applyVelocity(timeInterval);
    }
    #pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->update();
    }
    endTime = clock();
    cout << "Updating positions: " << std::to_string(endTime - startTime) << endl;
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
