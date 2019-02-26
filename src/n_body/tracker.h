//
// Created by jackcamp on 2/6/19.
//

#ifndef N_BODY_TRACKER_H
#define N_BODY_TRACKER_H

#include "simulation.h"


class tracker {
public:
    static tracker *instance();

    /**
     * Sets the simulation to actively request information from.
     */
    void setSimulation(simulation* theSim);

    /**
     * Marks the completion of a new frame of the simulation
     */
    void markFrameCompleted();

    /**
     * Markers for various events in the calculation of the frame
     */
    void markCalculationsStart();
    void markTreeCreated();
    void markTreeCompleted();
    void markCenterMassCalculated();
    void markGravityCalculated();
    void markVelocityApplied();
    void markPositionsUpdated();

    /**
     * Outputs information about the most recent frame calculated
     */
    void outputStatus();

private:
    static tracker *internalInstance; // The tracker contains one and only one instance of itself

    // This keeps a reference to the model being tracked
    simulation *theSim;

    // Data held by the tracker
    int numFrames;

    clock_t programStartTime;
    clock_t frameStartTime;
    clock_t timeStamp;

    clock_t calculationStartTime;
    clock_t treeCreationTime;
    clock_t treePopulationTime;
    clock_t centerMassCalculationTime;
    clock_t gravityCalculationTime;
    clock_t velocityApplicationTime;
    clock_t positionUpdateTime;
    clock_t totalCalculationTime;
    clock_t renderingTime;
    clock_t totalFrameTime;

    tracker(tracker const &) = default; // The copy constructor is private
    //tracker &operator=(tracker const &) {}; // The assignment operator is private
    tracker() = default; // Only the tracker is allowed to construct itself

    /**
     * Converts clock time to seconds
     * @param cycles Elapsed time in cpu clock cycles
     * @return Elapsed time in seconds
     */
    double inSeconds(clock_t cycles) {
        return cycles / (double) CLOCKS_PER_SEC;
    }
};


#endif //N_BODY_TRACKER_H
