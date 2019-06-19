//
// Created by jackcamp on 6/18/19.
//

#ifndef N_BODY_NEWBODY_H
#define N_BODY_NEWBODY_H

#include <string>

#include "../tools/Position.h"
#include "../tools/Velocity.h"
#include "../view/rendering/Renderer.h"

class newBody {

public:

    // Constructor

    newBody();


    // Setters / Named parameters

    /**
     * Sets the position of the body
     * @param position The position to set
     * @return This body, for use in chaining named parameters.
     */
    newBody *setPosition(Position position);

    /**
     * Sets the velocity of the body
     * @param velocity The velocity to set
     * @return This body, for use in chaining named parameters.
     */
    newBody *setVelocity(Velocity velocity);

    /**
     * Sets the mass of the body
     * @param mass Floating point mass of the body
     * @return This body, for use in chaining named parameters.
     */
    newBody *setMass(float mass);

    /**
     * Sets fixed to true, preventing forces from changing the body's path
     * Useful when you want a 'sun' Body to stay in place no matter what
     * @param fixed set to false to un-fix the body
     * @return This body, for use in chaining named parameters.
     */
    newBody *makeFixed(bool fixed = true);

    /**
     * Sets passive to true, preventing the body from affecting others
     * Useful for bodies that indicate forces for little additional performance cost
     * @param passive set to false to make the body active
     * @return This body, for use in chaining named parameters.
     */
    newBody *makePassive(bool passive = true);

    /**
     * Sets the object responsible for drawing this body
     * @param renderer the new renderer
     * @return This body, for use in chaining named parameters.
     */
    newBody *setRenderer(Renderer renderer);


    // Getters

    /**
     * Getter for the body's position
     * @return The current position of the body
     */
    Position getPosition();

    /**
     * Getter for the body's velocity
     * @return The current velocity of the body
     */
    Velocity getVelocity();

    /**
     * Getter for the body's mass
     * @return The mass of the body
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


    // Operators

    /**
     * Updates the position and velocity of the body to match their calculated next values
     */
    void shiftBuffers();


    // Graphics

    /**
     * Getter for the renderer of the body
     * @return This body's renderer
     */
    Renderer getRenderer();


    // Serialization

    /**
     * Converts the body to a string
     * @return A string describing the body
     */
    std::string toString();

    /**
     * Used by Cereal to turn the body into a serial archive
     * @tparam Archive
     * @param ar
     */
    template<class Archive>
    void serialize(Archive & ar);

private:

    // Used in physics calculations

    Position position = Position(0, 0, 0);
    Position nextPosition = Position(0, 0, 0);

    Velocity velocity = Velocity(0, 0, 0);
    Velocity nextVelocity = Velocity(0, 0, 0);

    float mass = 1;

    bool fixed = false;
    bool passive = false;


    // Used for drawing

    Renderer renderer;

};





#endif //N_BODY_NEWBODY_H
