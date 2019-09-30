//
// Created by jackcamp on 4/5/19.
//

#ifndef N_BODY_PHYSICSCONTEXT_H
#define N_BODY_PHYSICSCONTEXT_H


#include <glm/geometric.hpp>

#include "Body.h"

class PhysicsContext {

public:

    /**
     * Constructor for the physics context, every parameter starts with a default
     */
    PhysicsContext();

    // Setting physical constants of the simulation

    /**
     * Sets the constant of gravity
     * @param G The constant 'G' as a float
     * @return This simulation, for use in chaining named parameters.
     */
    PhysicsContext *setG(float G);

    /**
     * Sets the power distance is raised to when calculating the force of gravity
     * @param Power the new power of the simulation
     * @return This simulation, for use in chaining named parameters.
     */
    PhysicsContext *setPower(int Power);

    /**
     * Sets the time interval of the simulation
     * @param T Unit-less time interval
     * @return This simulation, for use in chaining named parameters.
     */
    PhysicsContext *setT(float T);


    // Getters for physical constants

    float getG() const;

    int getExponent() const;

    float getT() const;


    // Setting factors controlling the mechanics of the simulation

    /**
     * Selects whether Force Softening is enabled, off by default
     * @return This simulation, for use in chaining named parameters.
     */
    PhysicsContext *enableForceSoftening(bool enabled = true);

    /**
     * Sets the shortest time interval to use
     * @param minimumT The smallest time granularity allowed when super-sampling is enabled
     * @return This simulation, for use in chaining named parameters.
     */
    PhysicsContext *setMinimumT(float minimumT);


    // Methods used by other objects

    /**
     * Applies the gravity on the first body from the second and changes the velocity accordingly
     * @param subject the body being affected
     * @param actorPosition the position of the body creating the force
     * @param actorPosition the mass of the body creating the force
     */
    void applyGravityBetweenBodies(Body *subject, glm::vec3 actorPosition, float actorMass);

    /**
     * Applies the gravity on the first body from the second based on a specific time interval
     * @param subject the body being affected
     * @param actorPosition the position of the body creating the force
     * @param actorPosition the mass of the body creating the force
     * @param time the period the force is calculated over
     */
    void applyGravityBetweenBodies(Body *subject, glm::vec3 actorPosition, float actorMass, float time);


    // Helper methods used when setting up the simulation

    /**
     * Allows the user to force a body to orbit another by changing its velocity
     * @param sunBody The body to orbit
     * @param satelliteBody The body orbiting
     */
    void orbit(Body *sunBody, Body *satelliteBody);

    template<class Archive>
    void serialize(Archive & ar) {
        ar(
                CEREAL_NVP(G),
                CEREAL_NVP(Exponent),
                CEREAL_NVP(T)
        );
    }

private:


    // Values defining forces

    /*The gravitational constant (Higher values correspond to stronger gravity*/
    float G = 1.0;

    /*The power distance is raised to when calculating the force of gravity*/
    int Exponent = 2;


    // Values determining accuracy

    /*The time interval that forces are calculated with respect to*/
    float T = 1.0;

    /*Flag for whether or not super-sampling of forces should be used in cases of extreme acceleration*/
    bool ForceSofteningEnabled = false; // TODO Not yet implemented
    /*The smallest time granularity allowed when super-sampling is enabled*/
    float minimumT = 0.001;

    /*Flag telling the Simulation to account for collisions between bodies*/
    bool CollisionsEnabled = false;  // TODO Not yet implemented

};


#endif //N_BODY_PHYSICSCONTEXT_H
