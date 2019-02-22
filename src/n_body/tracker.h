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
    void recordFrame();

    /**
     * Outputs information about the most recent frame calculated
     */
    void reportFrame();

private:
    static tracker *internalInstance; // The tracker contains one and only one instance of itself

    // This keeps a reference to the simulation being tracked
    simulation *theSim;

    // Data held by the tracker
    int numFrames;
    clock_t timeStamp;
    clock_t frameTime;

    tracker(tracker const &) = default; // The copy constructor is private
    //tracker &operator=(tracker const &) {}; // The assignment operator is private
    tracker() = default; // Only the tracker is allowed to construct itself


};


#endif //N_BODY_TRACKER_H
