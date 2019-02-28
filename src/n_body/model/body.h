//
// Created by jackcamp on 2/25/19.
//

#ifndef N_BODY_BETTER_BODY_H
#define N_BODY_BETTER_BODY_H


#include <string> // Used by the toString method
#include <glm/vec3.hpp> // Used for holding 3d vectors

#include "../interface/drawable.h" // Body extends drawable
class viewport; // The draw method uses tools defined by the viewport class

/**
 * Intended to streamline the functionality of the old body
 * The goal is to retain all the same features, while reducing the number of methods.
 */
class body : public drawable {
public:

    // Constructor

    /**
     * The constructor only takes in the position, all other values start at their defaults.
     * @param position The starting location of the body (as a three dimensional vector)
     */
    explicit body(glm::vec3 position);


    // Setters (to be used as named parameters)

    /**
     * Sets the starting velocity of the body to something other than <0, 0, 0>
     * @param velocity The velocity to set
     * @return This body, for use in chaining named parameters.
     */
    body *setVelocity(glm::vec3 velocity);

    /**
     * Sets the mass of the body to something other than 100
     * @param mass the mass to set
     * @return This body, for use in chaining named parameters.
     */
    body *setMass(float mass);

    /**
     * Sets the density of the object used to calculate radius
     * @param density the new density
     * @return This body, for use in chaining named parameters.
     */
    body *setDensity(float density);

    /**
     * Sets fixed to true, locking the body's location
     * @return This body, for use in chaining named parameters.
     */
    body *makeFixed();

    /**
     * Sets passive to true, preventing the body from affecting others
     * @return This body, for use in chaining named parameters.
     */
    body *makePassive();

    /**
     * Sets the color of the body to something other than <1, 1, 1>
     * @param color the color to set
     * @return This body, for use in chaining named parameters.
     */
    body *setColor(glm::vec3 color);

    /**
     * Forces the body to be drawn with a specific radius, regardless of its density
     * @param radius the new radius of the body
     * @return This body, for use in chaining named parameters.
     */
    body *setRadius(float radius);


    // Getters

    /**
     * Getter for the location of the body
     * @return the position of the body as a vector
     */
    glm::vec3 getPosition();

    /**
     * Getter for the next location of the body
     * @return the (future) position of the body as a vector
     */
    glm::vec3 getNextPosition();

    /**
     * Getter for the velocity of the body
     * @return the velocity of the body as a vector
     */
    glm::vec3 getVelocity();

    /**
     * Getter for the next velocity of the body
     * @return the (future) velocity of the body as a vector
     */
    glm::vec3 getNextVelocity();

    /**
     * Getter for the mass of the body
     * @return the mass as a float
     */
    float getMass();

    /**
     * Getter for the value of the fixed flag
     * @return Whether the body is fixed in place or not
     */
    bool isFixed();

    /**
     * Getter for the value of the passive flag
     * @return Whether the body can affect others or not
     */
    bool isPassive();


    // Physics operators

    /**
     * Adds to the velocity of the body
     * @param deltaV the change in velocity as a vector
     */
    void kick(glm::vec3 deltaV);

    /**
     * Calculates the next position of the body based on its velocity
     * @param deltaT the change in time to calculate the shift with respect to
     */
    void drift(float deltaT);

    /**
     * Updates the position and velocity of the body to match their calculated next values
     */
    void shiftBuffers();


    // Graphics & Diagnostics

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
    float radius = 10.0;

    /*Used only to generate the radius based on the mass*/
    float density = 1.0;


    // Helper methods

    /**
     * Sets the radius according to the mass and the density
     */
    void calculateRadius();

};


#endif //N_BODY_BETTER_BODY_H