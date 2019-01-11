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

    //octree = new octant(vec3(0, 0, 0), 100000);

}

void simulation::addBody(body *newBody) {

    // Adds the new body to the collection
    bodies.push_back(newBody);
}

void simulation::increment() {

    clock_t time_a = clock();

    // Creates the Barnes-Hut Octree
    std::unique_ptr<octant> octree(new octant(vec3(0, 0, 0), 100000));

    // Populates the Barnes-Hut Octree
    for (body *theBody : bodies) {
        octree->addBody(theBody->getPosition(), theBody->getMass());
    }

    // Calculates center of mass data for non-leaf nodes of the tree
    octree->calculateCenterMass();

    // Applying gravity to each body
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {
        if (!bodies[b]->isFixed()) {
            octree->applyGravity(bodies[b], 0.5, this);
        }
    }

    // Updates each body's position
    #pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->applyVelocity(timeInterval);
    }

    clock_t time_b = clock();

    cout << std::to_string(time_b - time_a) << endl;
}

void simulation::applyGravity(body *passive, vec3 activePosition, float activeMass) {

    // Calculating the directionless force of gravity
    float forceOfGravity =
            (gravitationalConstant * passive->getMass() * activeMass) /
            ((float) pow(glm::distance(passive->getPosition(), activePosition), power) + 0.0001f);

    // Giving the force direction
    glm::vec3 force = forceOfGravity * glm::normalize(activePosition - passive->getPosition());

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
