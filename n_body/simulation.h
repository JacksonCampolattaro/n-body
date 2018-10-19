//
// Created by JackCamp on 8/6/18.
//

#ifndef N_BODY_WX_SIMULATION_H
#define N_BODY_WX_SIMULATION_H

// Make sure to install the necessary packages before using:
// glew glew-devel SDL2-devel SDL2_image-devel glm-devel freetype-devel glut freeglut-devel glm-devel
// CMakeLists.txt must contain 'target_link_libraries(cs455openGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})'
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

// Other libraries to include.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>


// My own classes
#include "body.h"


class simulation {
private:

    // Simulation parameters
    float gravitationalConstant;
    float physicsTimeIncrement;

    // Vector of pointers to bodies.
    std::vector<body *> bodies;

    int physicsIteration = 0;

public:


    //------------------------------------------------------------------------------------------------------------------------------
    // Simulation constructor
    //------------------------------------------------------------------------------------------------------------------------------

    // Simulation constructor
    simulation(float gravitationalConstant, float physicsTimeIncrement) {

        // Sets simulation parameters
        this->gravitationalConstant = gravitationalConstant;
        this->physicsTimeIncrement = physicsTimeIncrement;
    }

    // Allows for changing parameters after creating the simulation.
    void setParameters(float gravitationalConstant, float physicsTimeIncrement) {

        // Sets simulation parameters
        this->gravitationalConstant = gravitationalConstant;
        this->physicsTimeIncrement = physicsTimeIncrement;
    }



    //------------------------------------------------------------------------------------------------------------------------------
    // Body creation
    //------------------------------------------------------------------------------------------------------------------------------

    // Adds a new body to the simulation using prebuilt vectors as position and velocity.
    void addBody(vec3 position, vec3 velocity, float mass) {

        // Adds the body to the body array.
        bodies.push_back(new body(position, velocity, mass));

    }

    // Adds a new body to the simulation
    void addBody(float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ,
                 float mass, float density, vec3 color, bool fixed) {

        // Arranges position and velocity into vectors
        vec3 position(positionX, positionY, positionZ);
        vec3 velocity(velocityX, velocityY, velocityZ);

        // Adds the body to the body array.
        bodies.push_back(new body(position, velocity, mass, density, color, fixed));

    }

    // Adds a new body to the simulation
    void addBody(float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ,
                 float mass, float density, vec3 color) {

        // Arranges position and velocity into vectors
        vec3 position(positionX, positionY, positionZ);
        vec3 velocity(velocityX, velocityY, velocityZ);

        // Adds the body to the body array.
        bodies.push_back(new body(position, velocity, mass, density, color));

    }

    // Adds a new body to the simulation using prebuilt vectors as position and velocity.
    void addBody(vec3 position, vec3 velocity, float mass, float density, vec3 color) {

        // Adds the body to the body array.
        bodies.push_back(new body(position, velocity, mass, density, color));

    }

    // Adds a new body to the simulation using prebuilt vectors as position and velocity.
    void addBody(vec3 position, vec3 velocity, float mass, float density, vec3 color, bool fixed) {

        // Adds the body to the body array.
        bodies.push_back(new body(position, velocity, mass, density, color, fixed));

    }



    //------------------------------------------------------------------------------------------------------------------------------
    // Physics functions
    //------------------------------------------------------------------------------------------------------------------------------

    // Applies velocity to all bodies
    void applyVelocity() {

        for (body *thisBody : bodies) {
            (*thisBody).applyVelocity(physicsTimeIncrement);
        }
    }

    // Applies gravity to all bodies
    void applyGravity() {


        // Brute force system
        // Inefficient but easily parallelized
        ///*
        // Parallelizes the for loop
#pragma omp parallel for
#pragma acc kernels -ta=nvidia, time
        for (int thisBody = 0; thisBody < bodies.size(); ++thisBody) {
            for (int otherBody = 0; otherBody < bodies.size(); ++otherBody) {
                // Prevents a body from being affected by its own gravity.
                if (thisBody != otherBody) {
                    bodies[thisBody]->applyGravity(*bodies[otherBody], physicsTimeIncrement,
                                                   gravitationalConstant);
                }
            }
        }
        //*/

        // 'Mutual gravity' implementation
        // Best for single threaded performance but can only be done sequentially.
        /*
        int fullyCalculatedBodies = 0;

        // Parallelizes the for loop
        #pragma omp parallel for
        for (int thisBody = 0; thisBody < bodies.size(); ++thisBody) {
            for (int otherBody = fullyCalculatedBodies + 1; otherBody < bodies.size(); ++otherBody) {
                if (thisBody != otherBody) {
                    bodies[thisBody]->applyMutualGravity(bodies[otherBody], physicsTimeIncrement,
                                                          gravitationalConstant);
                }
            }
            ++fullyCalculatedBodies;
        }
        //*/

        ++physicsIteration;
    }

    // Increments the simulation
    void incrementPhysics() {

        applyVelocity();
        applyGravity();

        // Diagnostics in console
        //soundOff();

        //++physicsIteration;
    }

    int getPhysicsIteration() {

        return physicsIteration;
    }



    //------------------------------------------------------------------------------------------------------------------------------
    // Rendering and Diagnostics
    //------------------------------------------------------------------------------------------------------------------------------

    vector<vec3> getPositions() {
        vector<vec3> positions;

        for (body *thisBody : bodies) {
            positions.push_back(thisBody->getPosition());
        }

        return positions;
    }

    // Returns the vector containing all the bodies.
    vector<body *> getBodies() {
        return bodies;
    }

    // Tells all bodies to state their properties in the console
    void soundOff() {

        cout << "~~~~~~~~~~ \n";
        for (body *thisBody : bodies) {
            (*thisBody).soundOff();
        }
    }


};


#endif //N_BODY_WX_SIMULATION_H
