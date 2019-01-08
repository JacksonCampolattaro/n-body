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

    // Adds relationships to link the new body to each of the other bodies in the array.
    /*for (body *theBody : bodies) {
        relationships.push_back(new relationship(theBody, newBody));
    }*/

    // Adds the new body to the collection
    bodies.push_back(newBody);
}

void simulation::increment() {

    // Clearing all data from last cycle
    relationships.clear();
    relationships.reserve(bodies.size() * bodies.size());
    octree = new octant(vec3(0, 0, 0), 100000);

    // Populates the Barnes-Hut Octree
    for (body *theBody : bodies) {
        octree->addBody(theBody);
    }

    // Calculates center of mass data for non-leaf nodes of the tree
    octree->calculateCenterMass();

    // Array of relationships lists sorted by body (For thread safety)
    std::vector<relationship *> independentRelationships[bodies.size()];

    // Generates reduced relationship list using octants
    #pragma omp parallel for
    for (int b = 0; b < bodies.size(); ++b) {

        // Adding each list of relationships to the array
        independentRelationships[b] = octree->getRelationships(bodies[b], 0.9);
    }

    // Combining the lists by adding each one to the master list
    for (int c = 0; c < bodies.size(); ++c) {

        // Adding each list of relationships to the array
        relationships.insert(std::end(relationships), std::begin(independentRelationships[c]),
                             std::end(independentRelationships[c]));
    }

    cout << relationships.size() << " Relationships\n";

    // Updates each Relationship
    #pragma omp parallel for
    for (int i = 0; i < relationships.size(); ++i) {
        relationships[i]->applyGravity(timeInterval, gravitationalConstant, power);
    }

    // Updates each body
    #pragma omp parallel for
    for (int j = 0; j < bodies.size(); ++j) {
        bodies[j]->applyVelocity(timeInterval);
    }
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
