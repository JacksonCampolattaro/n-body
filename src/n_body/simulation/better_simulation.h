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
     * @param T Unit-less time interval
     * @return This simulation, for use in chaining named parameters.
     */
    better_simulation *setT(float T);

    /**
     * Selects whether Barnes-Hut optimizations is enabled, off by default
     * @return This simulation, for use in chaining named parameters.
     */
    better_simulation *enableSuperSampling(bool enabled = true);

    /**
     * Sets the shortest time interval to use
     * @param T The smallest time granularity allowed when super-sampling is enabled
     * @return This simulation, for use in chaining named parameters.
     */
    better_simulation *setMinimumT(float T);

    /**
     * Selects whether Barnes-Hut optimizations is enabled, on by default
     * @return This simulation, for use in chaining named parameters.
     */
    better_simulation *enableBarnesHut(bool enabled = true);

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

    /*Flag for whether or not super-sampling should be used in cases of extreme acceleration*/
    bool SuperSamplingEnable = false;

    /*The smallest time granularity allowed when super-sampling is enabled*/
    float minimumT = 0.001;

    /*Flag for whether or not the barnes-hut tree should be used*/
    bool BarnesHutEnable = true;

    /*Ratio defining how far any group of bodies must be before they can be grouped together*/
    float Theta = 0.8;

};


#endif //N_BODY_BETTER_SIMULATION_H
