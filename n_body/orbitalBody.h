//
// Created by JackCamp on 8/2/18.
//

#ifndef N_BODY_BODY_H
#define N_BODY_BODY_H

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "drawable.h"

class viewport;

using namespace std;
using namespace glm;

/**
 * The gravitational bodies which interact with each other in the simulation.
 */
class orbitalBody : public drawable {

public:

    /**
     * Constructor including default values. All values are without units.
     *
     * @param position the position coordinates of the body as a vector
     * @param velocity the velocity coordinates of the body as a vector
     * @param mass the mass of the body
     * @param density the density of the body
     * @param color the color of the body
     * @param fixed whether or not the body can be affected by gravity
     */
    orbitalBody(vec3 position, vec3 velocity, float mass = 100, float density = 100, vec3 color = vec3(1, 1, 1),
                bool fixed = false);


    /**
     * Updates position according to current velocity and a time interval
     *
     * @param interval unit-less time interval over which velocity is applied
     */
    void applyVelocity(float interval);

    /**
     * Applies a force to the body and updates the velocity accordingly.
     *
     * @param force the force applied
     */
    void applyForce(vec3 force, float interval);


    /**
     * Adds a velocity vector to the body.
     *
     * @param additionalVelocity the additional velocity as a vector
     */
    void addVelocity(vec3 additionalVelocity);

    /**
     * Getter for the body's position
     * @return the position of the body
     */
    vec3 getPosition();

    /**
     * Getter for the body's velocity
     * @return the velocity of the body
     */
    vec3 getVelocity();

    /**
     * Getter for the body's mass
     * @return the mass of the body
     */
    float getMass();

    /**
     * Getter for the body's radius
     * @return the radius of the body
     */
    float getRadius();

    /**
     * Getter for the body's color
     * @return the color of the body
     */
    vec3 getColor();

    /**
     * Outputs information about the body to the console
     */
    void soundOff();

    /**
     * Draws a circle at the location of the body
     */
    void draw() override;

private:
    // Coordinates

    vec3 position;
    vec3 velocity;


    // Properties

    float mass;
    float radius;

    vec3 color;

    bool fixed = false;

};


#endif //N_BODY_BODY_H
