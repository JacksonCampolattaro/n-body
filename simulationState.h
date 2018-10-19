//
// Created by jackcamp on 10/19/18.
//

#ifndef N_BODY_HEADLESS_SIMULATIONSTATE_H
#define N_BODY_HEADLESS_SIMULATIONSTATE_H

/**
 * Contains the current state of the simulation
 * and has methods for updating.
 */
class simulationState {

public:

    /**
     * Constructor for a simulation state
     */
    simulationState(float gravitationalConstant, float timeInterval) {

        this->gravitationalConstant = gravitationalConstant;
        this->timeInterval = timeInterval;

    }

    /**
     * Increments the simulation by applying physics to all bodies
     */
    void increment() {


    }


private:

    // Simulation parameters
    float gravitationalConstant;
    float timeInterval;

    // Contains all bodies
    std::vector<body *> bodies;

    // Contains all relationships between bodies
    std::vector<relationship *> relationships;

};


#endif //N_BODY_HEADLESS_SIMULATIONSTATE_H
