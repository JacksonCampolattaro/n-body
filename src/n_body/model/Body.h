//
// Created by jackcamp on 2/25/19.
//

#ifndef N_BODY_BETTER_BODY_H
#define N_BODY_BETTER_BODY_H


#include <string> // Used by the toString method
#include <glm/vec3.hpp> // Used for holding 3d vectors
#include <cereal/archives/xml.hpp>

#include "../tools/Position.h"
#include "../tools/Velocity.h"
#include "../tools/rgbaColor.h"

class Body {
public:

    // Constructor

    /**
     * The constructor only takes in the position, all other values start at their defaults.
     * @param position The starting location of the body (as a three dimensional vector)
     */
    explicit Body(Position position = Position(0, 0, 0));


    // Setters (to be used as named parameters)

    /**
     * Sets the starting velocity of the body to something other than <0, 0, 0>
     * @param velocity The velocity to set
     * @return This body, for use in chaining named parameters.
     */
    Body *setVelocity(Velocity velocity);

    /**
     * Sets the mass of the body to something other than 100
     * @param mass the mass to set
     * @return This body, for use in chaining named parameters.
     */
    Body *setMass(float mass);

    /**
     * Sets the density of the object used to calculate radius
     * @param density the new density
     * @return This body, for use in chaining named parameters.
     */
    Body *setDensity(float density);

    /**
     * Sets fixed to true, locking the body's location
     * @return This body, for use in chaining named parameters.
     */
    Body *makeFixed();

    /**
     * Sets passive to true, preventing the body from affecting others
     * @return This body, for use in chaining named parameters.
     */
    Body *makePassive();

    /**
     * Sets the color of the body to something other than <1, 1, 1>
     * @param color the color to set
     * @return This body, for use in chaining named parameters.
     */
    Body *setColor(rgbaColor color);

    /**
     * Forces the body to be drawn with a specific radius, regardless of its density
     * @param radius the new radius of the body
     * @return This body, for use in chaining named parameters.
     */
    Body *setRadius(float radius);


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
    void draw();

    /**
     * Converts the body to a string
     * @return a string describing the body
     */
    std::string toString();

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
                CEREAL_NVP(position),
                CEREAL_NVP(velocity),
                CEREAL_NVP(mass), CEREAL_NVP(fixed), CEREAL_NVP(passive),
                CEREAL_NVP(color),
                CEREAL_NVP(radius), CEREAL_NVP(density)
                );

        this->nextVelocity = this->velocity;
    }

private:

    // Core properties

    /*Core properties of the Body*/
    Position position;
    Velocity velocity = glm::vec3(0, 0, 0);
    float mass = 100.0;

    /*By updating these instead of original values, order of operations doesn't affect results*/
    Position nextPosition;
    Velocity nextVelocity = glm::vec3(0, 0, 0);


    // Simulation properties

    /*Fixed bodies are unaffected by all the forces that would act on them*/
    bool fixed = false; /*Useful when you want a 'sun' Body to stay in place no matter what*/

    /*Passive bodies do not have any affect on the bodies around them*/
    bool passive = false; /*Useful for bodies that indicate forces for little additional performance cost*/


    // Aesthetic properties

    /*rgbaColor is stored as a vector of floats, in the format <r, g, b, a>, white by default*/
    rgbaColor color = rgbaColor(1, 1, 1, 1);

    /*The radius of the circle representing the Body*/
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
