//
// Created by jackcamp on 2/6/19.
//

#include <iostream>

#include "tracker.h"

tracker *tracker::internalInstance = nullptr;

tracker *tracker::instance() {

    if (!internalInstance) {
        internalInstance = new tracker();

        internalInstance->numFrames = 0;
    }
    return internalInstance;
}

void tracker::setSimulation(simulation *theSim) {
    this->theSim = theSim;
}

void tracker::markFrameCompleted() {

    renderingTime = clock() - timeStamp;

    numFrames++;

    // Getting time to calculate frame
    totalFrameTime = clock() - startTime;

    // Resetting values for next frame
    startTime = clock();
    timeStamp = startTime;
}

void tracker::markTreeCompleted() {

    treePopulationTime = clock() - timeStamp;
    timeStamp = clock();
}

void tracker::markCenterMassCalculated() {
    centerMassCalculationTime = clock() - timeStamp;
    timeStamp = clock();
}

void tracker::markGravityCalculated() {
    gravityCalculationTime = clock() - timeStamp;
    timeStamp = clock();
}

void tracker::markVelocityApplied() {
    velocityApplicationTime = clock() - timeStamp;
    timeStamp = clock();
}

void tracker::markPositionsUpdated() {
    positionUpdateTime = clock() - timeStamp;
    timeStamp = clock();
}


void tracker::outputStatus() {

    cout << "Frame " << numFrames << ": " << endl;
    cout << "      Number of bodies:                    " << theSim->getNumBodies() << endl;
    cout << "      Total calculation time:              " << totalFrameTime << " clock cycles --> "
         << inSeconds(totalFrameTime) << " s" << endl;
    cout << "           Tree Population:                " << treePopulationTime << " clock cycles --> "
         << inSeconds(treePopulationTime) << " s" << endl;
    cout << "           Center of Mass Propegation:     " << centerMassCalculationTime << " clock cycles --> "
         << inSeconds(centerMassCalculationTime) << " s" << endl;
    cout << "           Gravitational Calculations:     " << gravityCalculationTime << " clock cycles --> "
         << inSeconds(gravityCalculationTime) << " s" << endl;
    cout << "           Application of Velocity:        " << velocityApplicationTime << " clock cycles --> "
         << inSeconds(velocityApplicationTime) << " s" << endl;
    cout << "           Updating Positions:             " << positionUpdateTime << " clock cycles --> "
         << inSeconds(positionUpdateTime) << " s" << endl;
    cout << "           Rendering:                      " << renderingTime << " clock cycles --> "
         << inSeconds(renderingTime) << " s" << endl;
    cout << "      Framerate (un-smoothed):             " << 1 / inSeconds(totalFrameTime) << " FPS" << endl;
    cout << endl;
}

