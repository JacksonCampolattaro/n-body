//
// Created by JackCamp on 8/2/18.
//

#ifndef N_BODY_BODY_H
#define N_BODY_BODY_H

#include <iostream>
#include <string>

// Make sure to install these before using; they should be available from the standard repositories.
#include <glm/glm.hpp>
#include <GL/glew.h>

using namespace std;
using namespace glm;


class body {
private:
    // Coordinates

    vec3 position;
    vec3 velocity;


    // Properties

    float mass;
    float radius;

    vec3 color;

    bool fixed = false;

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
    body(vec3 position, vec3 velocity, float mass = 100, float density = 100, vec3 color = vec3(1, 1, 1),
         bool fixed = false) {

        // Setting coordinate values
        this->position = position;
        this->velocity = velocity;

        // Setting volume based on mass and density
        float volume = mass / density;

        // Setting radius based on volume
        radius = pow((volume / float(M_PI)) * (3.0f / 4.0f), (1.0f / 3.0f));

        // Setting properties
        this->mass = mass;
        this->color = color;
        this->fixed = fixed;
    }


    /**
     * Updates position according to current velocity and a time interval
     *
     * @param interval unit-less time interval over which velocity is applied
     */
    void applyVelocity(float interval) {
        position += velocity * interval;
    }

    /**
     * Updates velocity according to the force of gravity and a time interval.
     *
     * @param otherBody the other mass
     * @param interval the time interval
     * @param gravitationalConstant the gravitational constant
     */
    void applyGravity(body otherBody, float interval, float gravitationalConstant) {

        if (!fixed) {

            // Gives the relationship between the two bodies as a unit vector.
            vec3 relationship = normalize(otherBody.getPosition() - position);

            // Force of gravity equation, returns a scalar.
            float forceOfGravity =
                    (gravitationalConstant * mass * otherBody.getMass()) / distance(position, otherBody.position);

            // Gives the force of gravity direction by multiplying it by a unit vector.
            vec3 force = forceOfGravity * relationship;

            // Converts force to acceleration.
            vec3 acceleration = force / mass;

            // Velocity is increased according to the acceleration and the time interval.
            addVelocity(acceleration * interval);

        }
    }

    /**
     * Adds a velocity vector to the body.
     *
     * @param additionalVelocity the additional velocity as a vector
     */
    void addVelocity(vec3 additionalVelocity) {
        velocity += additionalVelocity;
    }

    /**
     * Getter for the body's position
     * @return the position of the body
     */
    vec3 getPosition() {
        return position;
    }

    /**
     * Getter for the body's velocity
     * @return the velocity of the body
     */
    vec3 getVelocity() {
        return velocity;
    }

    /**
     * Getter for the body's mass
     * @return the mass of the body
     */
    float getMass() {
        return mass;
    }

    /**
     * Getter for the body's radius
     * @return the radius of the body
     */
    float getRadius() {
        return radius;
    }

    /**
     * Getter for the body's color
     * @return the color of the body
     */
    vec3 getColor() {
        return color;
    }

    /**
     * Outputs information about the body to the console
     */
    void soundOff() {

        // Prints position, velocity, and mass data in a readable format.
        cout << "[ x Position: " + to_string(position.x) + " | y Position: " + to_string(position.y) +
                " | z Position: " + to_string(position.z) + " ]\n";
        cout << "[ x Velocity: " + to_string(velocity.x) + " | y Velocity: " + to_string(velocity.y) +
                " | z Velocity: " + to_string(velocity.z) + " ]\n";
        cout << "[ Mass: " + to_string(mass) + " ] \n\n";
    }

};


#endif //N_BODY_BODY_H
