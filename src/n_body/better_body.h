//
// Created by jackcamp on 2/25/19.
//

#ifndef N_BODY_BETTER_BODY_H
#define N_BODY_BETTER_BODY_H


#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "drawable.h"

/**
 * Intended to streamline the functionality of the old body
 * The goal is to retain all the same features, while reducing the number of methods.
 */
class better_body : public drawable {
public:

    /**
     * The constructor only takes in the position, all other values start at their defaults.
     * @param position The starting location of the body (as a three dimensional vector)
     */
    better_body(glm::vec3 position);

    /**
     * Getter for the location of the body
     * @return the position of the body as a vector
     */
    glm::vec3 getPosition();

    /**
     * Sets the starting velocity of the body to something other than <0, 0, 0>
     * @param velocity The velocity to set
     * @return This body, for use in chaining named parameters.
     */
    better_body *setVelocity(glm::vec3 velocity);

    /**
     * Adds to the velocity of the body
     * @param deltaV the change in velocity as a vector
     */
    void addVelocity(glm::vec3 deltaV);

    /**
     * Sets the mass of the body to something other than 100
     * @param mass the mass to set
     * @return This body, for use in chaining named parameters.
     */
    better_body *setMass(float mass);

    /**
     * Getter for the mass of the body
     * @return the mass as a float
     */
    float getMass();

    /**
     * Sets fixed to true, locking the body's location
     * @return This body, for use in chaining named parameters.
     */
    better_body *makeFixed();

    /**
     * Sets passive to true, preventing the body from affecting others
     * @return This body, for use in chaining named parameters.
     */
    better_body *makePassive();

    /**
     * Sets the starting velocity of the body to something other than <1, 1, 1>
     * @param color the color to set
     * @return This body, for use in chaining named parameters.
     */
    better_body *setColor(glm::vec3 color);

    /**
     * Updates the position of the body based on its velocity
     */
    void update();

    /**
     * Draws a circle at the location of the body
     */
    void draw() override;

    /**
     * Converts the body to a string
     * @return a string describing the body
     */
    std::string toString();

private:

    // Core properties

    /*Core properties of the body*/
    glm::vec3 position;
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    float mass = 100.0;

    /*By updating these instead of original values, order of operations doesn't affect results*/
    glm::vec3 nextPosition;
    glm::vec3 nextVelocity;


    // Simulation properties

    /*Fixed bodies are unaffected by all the forces that would act on them*/
    bool fixed = false; /*Useful when you want a 'sun' body to stay in place no matter what*/

    /*Passive bodies do not have any affect on the bodies around them*/
    bool passive = false; /*Useful for bodies that indicate forces for little additional performance cost*/


    // Aesthetic properties

    /*Color is stored as a vector of floats, in the format <r, g, b>, white by default*/
    glm::vec3 color = glm::vec3(1, 1, 1);

    /*The radius of the circle representing the body*/
    float radius;

    /*Used only to generate the radius based on the mass*/
    float density;

};


#endif //N_BODY_BETTER_BODY_H