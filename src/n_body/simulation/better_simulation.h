//
// Created by jackcamp on 2/25/19.
//

#ifndef N_BODY_BETTER_SIMULATION_H
#define N_BODY_BETTER_SIMULATION_H

/**
 * A more streamlined simulation class
 * I'm using the fluent interface paradigm to make it simpler to construct.
 */
class better_simulation {

public:

    /**
     * Constructor for the simulation, every parameter starts with a default
     */
    better_simulation();

    /**
     * Sets the constant of gravity
     * @param G The constant 'G' as a float
     * @return This simulation, for use in chaining named parameters.
     */
    better_simulation *setG(float G);

    /**
     * Sets the power distance is raised to when calculating the force of gravity
     * @param Power the new power of the simulation
     * @return This simulation, for use in chaining named parameters.
     */
    better_simulation *setPower(int Power);

    /**
     * Sets the time interval of the simulation
     * @param T Unitless time interval
     * @return This simulation, for use in chaining named parameters.
     */
    better_simulation *setT(float T);

    /**
     * Sets the accuracy of the simulation (lower is more accurate, 0 is naive)
     * @param Theta Ratio defining how far any group of bodies must be before they can be grouped together
     * @return This simulation, for use in chaining named parameters.
     */
    better_simulation *setTheta(float Theta);

private:

    // Constants defining forces

    /*The gravitational constant (Higher values correspond to stronger gravity*/
    float G = 1.0;

    /*The power distance is raised to when calculating the force of gravity*/
    int Power = 2;


    // Constants defining accuracy

    /*The time interval that forces are calculated with respect to*/
    float T = 1.0;

    /*Ratio defining how far any group of bodies must be before they can be grouped together*/
    float Theta = 0;
};


#endif //N_BODY_BETTER_SIMULATION_H
